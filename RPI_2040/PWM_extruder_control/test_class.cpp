#include <Arduino.h>
#include "test_class.h"

test::test(){
	Serial.begin(115200);
}

void test::print(){
	Serial.print("hello");
}
