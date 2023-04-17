#include <gtest/gtest.h>
#include "../include/od.h"
#include <iostream>
#include <fstream>

class ODTest : public ::testing::Test {
	protected:
	   OD od;	
};

TEST_F(ODTest, GetImageTest) {
	
	od.get_image();
	
	std::ifstream file("image.jpg");	
	EXPECT_TRUE(file.good());
}

TEST_F(ODTest, PerformODTest){
	od.predict();
	std::string od_result = od.get_predicted_label();
	EXPECT_NE(od_result, "");	
}
