#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(255);
	ofSetWindowTitle("Insta");

	this->cap.open("D:\\video\\MOV_0506.mp4");
	
	int fourcc = (int)this->cap.get(CV_CAP_PROP_FOURCC);
	double fps = this->cap.get(CV_CAP_PROP_FPS);
	cv::Size cap_size = cv::Size(this->cap.get(CV_CAP_PROP_FRAME_WIDTH), this->cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	this->writer = cv::VideoWriter("D:\\video\\out.mp4", fourcc, fps, cap_size);

	this->blend_image.allocate(cap_size.width, cap_size.height, OF_IMAGE_COLOR);
	this->blend_frame = cv::Mat(this->blend_image.getHeight(), this->blend_image.getWidth(), CV_MAKETYPE(CV_8UC3, this->blend_image.getPixels().getNumChannels()), this->blend_image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat src;
	this->cap >> src;
	if (src.empty()) {

		return;
	}
	cv::cvtColor(src, src, CV_RGB2BGR);

	if (ofGetFrameNum() < this->frame_number) {

		for (int i = ofGetFrameNum() - 1; i > 0; i--) {

			this->save_frame[i - 1].copyTo(this->save_frame[i]);
		}
		src.copyTo(this->save_frame[0]);
	}
	else {

		for (int i = this->frame_number - 1; i > 0; i--) {

			this->save_frame[i - 1].copyTo(this->save_frame[i]);
		}
		src.copyTo(this->save_frame[0]);
	}

	if (ofGetFrameNum() > this->frame_number) {

		for (int y = 0; y < this->blend_frame.rows; y += 1) {

			for (int x = 0; x < this->blend_frame.cols; x += 1) {

				int index = ofNoise(x * 0.003, y * 0.003, ofGetFrameNum() * 0.03) * this->frame_number;
				this->blend_frame.at<cv::Vec3b>(y, x) = this->save_frame[index].at<cv::Vec3b>(y, x);
			}
		}

		this->blend_image.update();
		cv::cvtColor(blend_frame, blend_frame, CV_RGB2BGR);
		writer << blend_frame;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	if (ofGetFrameNum() > this->frame_number) {

		this->blend_image.draw(0, 0);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp(new ofApp());
}