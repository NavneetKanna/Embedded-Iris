#include <gtest/gtest.h>
#include "../include/ocr.h"
#include <iostream>
#include <fstream>

class OCRTest : public ::testing::Test {
	protected:
	   OCR ocr;	
};

TEST_F(OCRTest, GetImageTest) {
	
	ocr.get_image();
	
	std::ifstream file("image.jpg");	
	EXPECT_TRUE(file.good());
}

TEST_F(OCRTest, PerformOcrTest){
	ocr.perform_ocr();
	std::string ocr_result = ocr.get_ocr_result();
	EXPECT_NE(ocr_result, "");	
}



