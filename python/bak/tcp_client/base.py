# Copyright 2012 Pinterest.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import errno
import socket
import six

import pool

from exceptions import (
    MemcacheClientError,
    MemcacheUnknownCommandError,
    MemcacheIllegalInputError,
    MemcacheServerError,
    MemcacheUnknownError,
    MemcacheUnexpectedCloseError
)


class Client(object):
    """
    A client for a single memcached server.

    *Server Connection*

     The ``server`` parameter controls how the client connects to the memcached
     server. You can either use a (host, port) tuple for a TCP connection or a
     string containing the path to a UNIX domain socket.

     The ``connect_timeout`` and ``timeout`` parameters can be used to set
     socket timeout values. By default, timeouts are disabled.

     When the ``no_delay`` flag is set, the ``TCP_NODELAY`` socket option will
     also be set. This only applies to TCP-based connections.

     Lastly, the ``socket_module`` allows you to specify an alternate socket
     implementation (such as `gevent.socket`_).

     .. _gevent.socket: http://www.gevent.org/api/gevent.socket.html

    *Keys and Values*

     Keys must have a __str__() method which should return a str with no more
     than 250 ASCII characters and no whitespace or control characters. Unicode
     strings must be encoded (as UTF-8, for example) unless they consist only
     of ASCII characters that are neither whitespace nor control characters.

     Values must have a __str__() method to convert themselves to a byte
     string. Unicode objects can be a problem since str() on a Unicode object
     will attempt to encode it as ASCII (which will fail if the value contains
     code points larger than U+127). You can fix this with a serializer or by
     just calling encode on the string (using UTF-8, for instance).

     If you intend to use anything but str as a value, it is a good idea to use
     a serializer and deserializer. The pymemcache.serde library has some
     already implemented serializers, including one that is compatible with
     the python-memcache library.

    *Serialization and Deserialization*

     The constructor takes two optional functions, one for "serialization" of
     values, and one for "deserialization". The serialization function takes
     two arguments, a key and a value, and returns a tuple of two elements, the
     serialized value, and an integer in the range 0-65535 (the "flags"). The
     deserialization function takes three parameters, a key, value and flags
     and returns the deserialized value.

     Here is an example using JSON for non-str values:

     .. code-block:: python

         def serialize_json(key, value):
             if type(value) == str:
                 return value, 1
             return json.dumps(value), 2

         def deserialize_json(key, value, flags):
             if flags == 1:
                 return value

             if flags == 2:
                 return json.loads(value)

             raise Exception("Unknown flags for value: {1}".format(flags))

    *Error Handling*

     All of the methods in this class that talk to memcached can throw one of
     the following exceptions:

      * MemcacheUnknownCommandError
      * MemcacheClientError
      * MemcacheServerError
      * MemcacheUnknownError
      * MemcacheUnexpectedCloseError
      * MemcacheIllegalInputError
      * socket.timeout
      * socket.error

     Instances of this class maintain a persistent connection to memcached
     which is terminated when any of these exceptions are raised. The next
     call to a method on the object will result in a new connection being made
     to memcached.
    """

    def __init__(self,
                 server,
                 serializer=None,
                 deserializer=None,
                 connect_timeout=None,
                 timeout=None,
                 no_delay=False,
                 socket_module=socket):
        """
        Constructor.

        Args:
          server: tuple(hostname, port) or string containing a UNIX socket path.
          serializer: optional function, see notes in the class docs.
          deserializer: optional function, see notes in the class docs.
          connect_timeout: optional float, seconds to wait for a connection to
            the memcached server. Defaults to "forever" (uses the underlying
            default socket timeout, which can be very long).
          timeout: optional float, seconds to wait for send or recv calls on
            the socket connected to memcached. Defaults to "forever" (uses the
            underlying default socket timeout, which can be very long).
          no_delay: optional bool, set the TCP_NODELAY flag, which may help
            with performance in some cases. Defaults to False.
          ignore_exc: optional bool, True to cause the "get", "gets",
            "get_many" and "gets_many" calls to treat any errors as cache
            misses. Defaults to False.
          socket_module: socket module to use, e.g. gevent.socket. Defaults to
            the standard library's socket module.

        Notes:
          The constructor does not make a connection to memcached. The first
          call to a method on the object will do that.
        """
        self.server = server
        self.serializer = serializer
        self.deserializer = deserializer
        self.connect_timeout = connect_timeout
        self.timeout = timeout
        self.no_delay = no_delay
        self.socket_module = socket_module
        self.sock = None

    def _connect(self):
        self.close()

        if isinstance(self.server, (list, tuple)):
            sock = self.socket_module.socket(self.socket_module.AF_INET,
                                             self.socket_module.SOCK_STREAM)
        else:
            sock = self.socket_module.socket(self.socket_module.AF_UNIX,
                                             self.socket_module.SOCK_STREAM)
        try:
            sock.settimeout(self.connect_timeout)
            sock.connect(self.server)
            sock.settimeout(self.timeout)
            if self.no_delay and sock.family == self.socket_module.AF_INET:
                sock.setsockopt(self.socket_module.IPPROTO_TCP,
                                self.socket_module.TCP_NODELAY, 1)

            sock.setsockopt(self.socket_module.SOL_SOCKET,
                            self.socket_module.SO_SNDBUF, 10 * 1024 * 1024)

        except Exception:
            sock.close()
            raise

        self.sock = sock

    def close(self):
        """Close the connection to memcached, if it is open. The next call to a
        method that requires a connection will re-open it."""
        if self.sock is not None:
            try:
                self.sock.close()
            except Exception:
                pass
            finally:
                self.sock = None

    def quit(self):
        self.close()

    def sendMsg(self, s):
        if self.sock is None:
            self._connect()

        try:
            if self.sendHandler:
                self.sendHandler(self.sock, s)
            else:
                self.sock.sendall(s)

            # recv msg
            # s, size = self.recvHandler(self.sock)

        except Exception:
            self.close()
            raise

        #return 

    def recvMsg(self, s):
        return 

    def _raise_errors(self, line, name):
        if line.startswith(b'ERROR'):
            raise MemcacheUnknownCommandError(name)

        if line.startswith(b'CLIENT_ERROR'):
            error = line[line.find(b' ') + 1:]
            raise MemcacheClientError(error)

        if line.startswith(b'SERVER_ERROR'):
            error = line[line.find(b' ') + 1:]
            raise MemcacheServerError(error)

    def _misc_cmd(self, cmds, cmd_name, noreply):
        if self.sock is None:
            self._connect()

        try:
            self.sock.sendall(b''.join(cmds))

            if noreply:
                return []

            results = []
            buf = b''
            for cmd in cmds:
                buf, line = _readline(self.sock, buf)
                self._raise_errors(line, cmd_name)
                results.append(line)
            return results

        except Exception:
            self.close()
            raise


class PooledClient(object):
    """A thread-safe pool of clients (with the same client api).

    Args:
      max_pool_size: maximum pool size to use (going above this amount
                     triggers a runtime error), by default this is 2147483648L
                     when not provided (or none).
      lock_generator: a callback/type that takes no arguments that will
                      be called to create a lock or sempahore that can
                      protect the pool from concurrent access (for example a
                      eventlet lock or semaphore could be used instead)

    Further arguments are interpreted as for :py:class:`.Client` constructor.
    """

    def __init__(self,
                 server,
                 serializer=None,
                 deserializer=None,
                 connect_timeout=None,
                 timeout=None,
                 no_delay=False,
                 socket_module=socket,
                 max_pool_size=None,
                 lock_generator=None):
        self.server = server
        self.serializer = serializer
        self.deserializer = deserializer
        self.connect_timeout = connect_timeout
        self.timeout = timeout
        self.no_delay = no_delay
        self.socket_module = socket_module
        self.client_pool = pool.ObjectPool(
            self._create_client,
            after_remove=lambda client: client.close(),
            max_size=max_pool_size,
            lock_generator=lock_generator)

    def _create_client(self):
        client = Client(self.server,
                        serializer=self.serializer,
                        deserializer=self.deserializer,
                        connect_timeout=self.connect_timeout,
                        timeout=self.timeout,
                        no_delay=self.no_delay,
                        # We need to know when it fails *always* so that we
                        # can remove/destroy it from the pool...
                        socket_module=self.socket_module)
        return client

    def close(self):
        self.client_pool.clear()

    def set(self, key, value, expire=0, noreply=None):
        with self.client_pool.get_and_release(destroy_on_fail=True) as client:
            return client.set(key, value, expire=expire, noreply=noreply)


    def gets(self, key):
        with self.client_pool.get_and_release(destroy_on_fail=True) as client:
            try:
                return client.gets(key)
            except Exception:
                if self.ignore_exc:
                    return (None, None)
                else:
                    raise

    def stats(self, *args):
        with self.client_pool.get_and_release(destroy_on_fail=True) as client:
            try:
                return client.stats(*args)
            except Exception:
                if self.ignore_exc:
                    return {}
                else:
                    raise

    def version(self):
        with self.client_pool.get_and_release(destroy_on_fail=True) as client:
            return client.version()

    def flush_all(self, delay=0, noreply=None):
        with self.client_pool.get_and_release(destroy_on_fail=True) as client:
            return client.flush_all(delay=delay, noreply=noreply)

    def quit(self):
        with self.client_pool.get_and_release(destroy_on_fail=True) as client:
            try:
                client.quit()
            finally:
                self.client_pool.destroy(client)

    def __setitem__(self, key, value):
        self.set(key, value, noreply=True)

    def __getitem__(self, key):
        value = self.get(key)
        if value is None:
            raise KeyError
        return value

    def __delitem__(self, key):
        self.delete(key, noreply=True)


def _readline(sock, buf):
    """Read line of text from the socket.

    Read a line of text (delimited by "\r\n") from the socket, and
    return that line along with any trailing characters read from the
    socket.

    Args:
        sock: Socket object, should be connected.
        buf: String, zero or more characters, returned from an earlier
            call to _readline or _readvalue (pass an empty string on the
            first call).

    Returns:
      A tuple of (buf, line) where line is the full line read from the
      socket (minus the "\r\n" characters) and buf is any trailing
      characters read after the "\r\n" was found (which may be an empty
      string).

    """
    chunks = []
    last_char = b''

    while True:
        # We're reading in chunks, so "\r\n" could appear in one chunk,
        # or across the boundary of two chunks, so we check for both
        # cases.

        # This case must appear first, since the buffer could have
        # later \r\n characters in it and we want to get the first \r\n.
        if last_char == b'\r' and buf[0:1] == b'\n':
            # Strip the last character from the last chunk.
            chunks[-1] = chunks[-1][:-1]
            return buf[1:], b''.join(chunks)
        elif buf.find(b'\r\n') != -1:
            before, sep, after = buf.partition(b"\r\n")
            chunks.append(before)
            return after, b''.join(chunks)

        if buf:
            chunks.append(buf)
            last_char = buf[-1:]

        buf = _recv(sock, RECV_SIZE)
        if not buf:
            raise MemcacheUnexpectedCloseError()


def _readvalue(sock, buf, size):
    """Read specified amount of bytes from the socket.

    Read size bytes, followed by the "\r\n" characters, from the socket,
    and return those bytes and any trailing bytes read after the "\r\n".

    Args:
        sock: Socket object, should be connected.
        buf: String, zero or more characters, returned from an earlier
            call to _readline or _readvalue (pass an empty string on the
            first call).
        size: Integer, number of bytes to read from the socket.

    Returns:
      A tuple of (buf, value) where value is the bytes read from the
      socket (there will be exactly size bytes) and buf is trailing
      characters read after the "\r\n" following the bytes (but not
      including the \r\n).

    """
    chunks = []
    rlen = size + 2
    while rlen - len(buf) > 0:
        if buf:
            rlen -= len(buf)
            chunks.append(buf)
        buf = _recv(sock, RECV_SIZE)
        if not buf:
            raise MemcacheUnexpectedCloseError()

    # Now we need to remove the \r\n from the end. There are two cases we care
    # about: the \r\n is all in the last buffer, or only the \n is in the last
    # buffer, and we need to remove the \r from the penultimate buffer.

    if rlen == 1:
        # replace the last chunk with the same string minus the last character,
        # which is always '\r' in this case.
        chunks[-1] = chunks[-1][:-1]
    else:
        # Just remove the "\r\n" from the latest chunk
        chunks.append(buf[:rlen - 2])

    return buf[rlen:], b''.join(chunks)


def _recv(sock, size):
    """sock.recv() with retry on EINTR"""
    while True:
        try:
            return sock.recv(size)
        except IOError as e:
            if e.errno != errno.EINTR:
                raise
