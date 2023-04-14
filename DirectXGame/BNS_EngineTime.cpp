#include "BNS_EngineTime.h"

#include <thread>

BNS_EngineTime* BNS_EngineTime::sharedInstance = nullptr;

std::chrono::time_point<std::chrono::steady_clock> BNS_EngineTime::startTime = high_resolution_clock::now();
std::chrono::time_point<std::chrono::steady_clock> BNS_EngineTime::prevTime = high_resolution_clock::now();
std::chrono::time_point<std::chrono::steady_clock> BNS_EngineTime::fpsTime = high_resolution_clock::now();

double BNS_EngineTime::fps = 0.0f;
int BNS_EngineTime::frameCount = 0;
std::chrono::duration<double> BNS_EngineTime::targetFrameTime = std::chrono::duration<double>(1.0 / 180.0);

void BNS_EngineTime::initialize()
{
	sharedInstance = new BNS_EngineTime();

}

double BNS_EngineTime::getDeltaTime()
{
	return sharedInstance->deltaTime;
}

double BNS_EngineTime::getFPS()
{
    return sharedInstance->fps;
}

BNS_EngineTime::BNS_EngineTime()
{

}

BNS_EngineTime::~BNS_EngineTime()
{
	delete sharedInstance;
}

void BNS_EngineTime::LogFrameStart()
{
	sharedInstance->start = std::chrono::system_clock::now();
}

void BNS_EngineTime::LogFrameEnd()
{
	sharedInstance->end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = sharedInstance->end - sharedInstance->start;

	sharedInstance->deltaTime = elapsed_seconds.count();

	//std::cout << "Frame update finished in " << sharedInstance->deltaTime << "\n";
}

void BNS_EngineTime::CalculateFPS()
{
    // Calculate the time elapsed since the previous frame
    auto currentTime = high_resolution_clock::now();
    std::chrono::duration<double> deltaTime = currentTime - prevTime;

    // Cap the frame time to the target frame time
    if (deltaTime < targetFrameTime)
    {
        std::this_thread::sleep_for(targetFrameTime - deltaTime);
        deltaTime = targetFrameTime;
    }

    // Calculate FPS
    ++frameCount;
    if (currentTime - fpsTime >= std::chrono::seconds(1))
    {
        fps = static_cast<double>(frameCount) / std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - fpsTime).count();
        frameCount = 0;
        fpsTime = currentTime;
    }

    // Update the previous time
    prevTime = currentTime;

    // Do other game logic and rendering here
}
