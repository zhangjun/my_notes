#include <iostream>
#include <unordered_map>

#define DISABLE_COPY_AND_ASSIGN(classname)         \
 private:                                          \
  classname(const classname&) = delete;            \
  classname(classname&&) = delete;                 \
  classname& operator=(const classname&) = delete; \
  classname& operator=(classname&&) = delete

template <typename PassType>
struct PassRegistrar;

class Registrar {
 public:
  void Touch() { std::cout << "touch" << std::endl;}
};

class PassRegistry {
 public:
  static PassRegistry &Instance();

 private:
  PassRegistry() = default;

  DISABLE_COPY_AND_ASSIGN(PassRegistry);
};

template <typename PassType>
struct PassRegistrar : public Registrar {
  explicit PassRegistrar(const char *pass_type) {}
  PassRegistrar<PassType> &RequireAttr(const std::string &attr) {
    return *this;
  }
};

#define REGISTER_PASS(pass_type, pass_class)                \
  static PassRegistrar<pass_class> \
      __pass_registrar_##pass_type##__(#pass_type);         \
  int TouchPassRegistrar_##pass_type() {                    \
    __pass_registrar_##pass_type##__.Touch();               \
    return 0;                                               \
  }                                                         \
  static PassRegistrar<pass_class> \
      &__pass_tmp_registrar_##pass_type##__ UNUSED =        \
          __pass_registrar_##pass_type##__

#define USE_PASS(pass_type)                           \
  extern int TouchPassRegistrar_##pass_type();        \
  static int use_pass_itself_##pass_type##_ UNUSED =  \
      TouchPassRegistrar_##pass_type()


class Pass {};

// REGISTER_PASS(pass, Pass).RequireAttr("test");

// USE_PASS(pass);

int main() {
    static PassRegistrar<Pass> __pass_registrar_pass__("pass");
    __pass_registrar_pass__.Touch();
}