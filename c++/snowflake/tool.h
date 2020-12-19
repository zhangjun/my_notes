#pragma once 
#include <atomic>
#include <string>
#include <chrono>
#include <time.h>

namespace sogou {

class SnowFlake {
public:
	static SnowFlake* instance() {
		static SnowFlake sf;
		return &sf;
	}
	
	std::string GetId() {
		auto unix_tm = std::chrono::seconds(std::time(NULL)).count();
		return std::to_string(unix_tm) + std::string("01") + std::to_string((++ incr_) & seq_mask);
	}

	std::string GetIdV2() {
		long long timestamp = time_ms();
		if(last_time_ == timestamp) {
			incr = (++ incr_) &seq_mask;
			if(incr == 0) {
				timestamp = till_next_ms(last_time_);
			}
		} else {
			incr = 0;
		}
		last_time_ = timestamp;
		return (timestamp - twepoch) << shift_tm_ |
			(idc_id_ << shift_idc_) |
			(machine_id_ << shift_mid_) |
			incr;
	}
	
	long long time_ms() {
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return ms;
	}

private:
	// |1|41|5|5|12|
	static const long twepoch_ = 1420041600000L;
	static const long idc_bits_ = 5L;
	static const long mid_bits_ = 5L;
	static const long seq_bits_ = 12L;
	static const long max_mid_ = -1L ^ (-1L << mid_bits_) ;
	static const long max_idc_ = -1L ^ (-1L << idc_bits_) ;
	static const long max_seq_ = -1L ^ (-1L << seq_bits_) ;

	static const long shift_seq_ = seq_bits;   // 12
	static const long shitf_idc_ = shift_seq + mid_bits;  // 12 + 5
	static const long shift_tm_ = shift_idc + idc_bits;     // 12 + 5 + 5

	long seq_mask = -1L ^ (-1L << 12);

	long long last_time_;
	int machine_id_;
	int idc_id_;
	int product_id_;
	std::atomic<int> incr_;
};

}
