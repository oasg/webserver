#include <gtest/gtest.h>
#include<iostream>
#include "blockqueue.h"
#include "log.h"
// // Demonstrate some basic assertions.
TEST(BLOCKQUEUE_TEST, BasicAssertions){

    BlockQueue<int> b(1000);
    for(int i = 0; i < 1000; i++) {
        b.push_back(std::move(i));
    }
    ASSERT_EQ(1000, b.size());
}


TEST(LogTest, BasicAssertions) {
  int cnt = 0, level = 0;
    Log::getInst()->init(static_cast<log_level>(level), "./testlog1", ".log", 0);
    Log* log = Log::getInst();
    if(log->IsOpen()){
        log->write(log_level::DEBUG, "%s %d", "Hello Log", 100);
    }

    for(level = 3; level >= 0; level--) {
        Log::getInst()->SetLevel(static_cast<log_level>(level));
        for(int j = 0; j < 10000; j++ ){
            for(int i = 0; i < 4; i++) {
                LOG_BASE(static_cast<log_level>(i),"%s 111111111 %d ============= ", "Test", cnt++);
            }
        }
    }
    cnt = 0;
    Log::getInst()->init(static_cast<log_level>(level), "./testlog2", ".log", 5000);
    for(level = 0; level < 4; level++) {
        Log::getInst()->SetLevel(static_cast<log_level>(level));
        for(int j = 0; j < 10000; j++ ){
            for(int i = 0; i < 4; i++) {
                LOG_BASE(static_cast<log_level>(i),"%s 222222222 %d ============= ", "Test", cnt++);
            }
        }
    }
}