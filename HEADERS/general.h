#pragma once

constexpr unsigned short SCREEN_HEIGHT = 768;
constexpr unsigned short SCREEN_WIDTH = 1024;

constexpr unsigned short CELL_SIZE = 64;
constexpr unsigned short MAP_HEIGHT = (SCREEN_HEIGHT / CELL_SIZE);
constexpr unsigned short MAP_WIDTH = (SCREEN_WIDTH / CELL_SIZE);

const unsigned int FRAME_DURATION = 16667;
const float PI = 3.1415926f;
const float ONE_DEGREE_TO_RADIAN = 0.017453f;
const float RADIAN = 57.2958f;

const char PATH_MAP[] = "D:\\VisualStudioProjects\\map.txt";
