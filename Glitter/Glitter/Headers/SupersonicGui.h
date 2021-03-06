#pragma once

#ifndef SUPERGUI_H
#define SUPERGUI_H


#include <nanogui/nanogui.h>
#include <GLFW/glfw3.h>



using namespace nanogui;

struct sliderWithTextBox {
	Slider slider;
	TextBox* textBox;
};

class SupersonicGUI{
public:
	SupersonicGUI(GLFWwindow* window, std::function<void(float)> cb);

	~SupersonicGUI(){
		delete mScreen;
		delete gui;
	}
	void setFrameMetrics(int fps, float miliseconds);
	void addSlider(std::string label,  float min, float max,const std::function<void(float)> &cb);
	void draw();
	void refresh();

	TextBox* textBox;
	bool shouldSpin = true;
	bool bloom = false;
private:
	int fps;
	float ms;
	Screen* mScreen;
	FormHelper *gui;
};

#endif
