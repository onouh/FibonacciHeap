#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(QObject* parent)
    : QObject(parent), currentStep(-1), elapsedTime(0), isPlaying(false), animationSpeed(1) {
    
    timer = new QTimer(this);
    timer->setInterval(16);  // ~60 FPS
    connect(timer, &QTimer::timeout, this, &AnimationSystem::onTimerTick);
}

AnimationSystem::~AnimationSystem() {
    stop();
}

void AnimationSystem::addStep(const AnimationStep& step) {
    steps.push_back(step);
}

void AnimationSystem::clearSteps() {
    stop();
    steps.clear();
    currentStep = -1;
    elapsedTime = 0;
}

void AnimationSystem::play() {
    if (steps.empty()) return;
    
    isPlaying = true;
    currentStep = 0;
    elapsedTime = 0;
    timer->start();
    emit animationStarted();
    emit animationStepChanged(currentStep);
}

void AnimationSystem::pause() {
    if (isPlaying) {
        isPlaying = false;
        timer->stop();
    }
}

void AnimationSystem::resume() {
    if (!isPlaying && currentStep >= 0 && currentStep < static_cast<int>(steps.size())) {
        isPlaying = true;
        timer->start();
    }
}

void AnimationSystem::stop() {
    isPlaying = false;
    timer->stop();
    currentStep = -1;
    elapsedTime = 0;
}

void AnimationSystem::setSpeed(int speed) {
    animationSpeed = std::max(1, std::min(3, speed));
}

float AnimationSystem::getCurrentProgress() const {
    if (currentStep < 0 || currentStep >= static_cast<int>(steps.size())) {
        return 0.0f;
    }
    
    const AnimationStep& step = steps[currentStep];
    if (step.duration == 0) return 1.0f;
    
    float progress = static_cast<float>(elapsedTime) / static_cast<float>(step.duration);
    return std::min(1.0f, progress);
}

const AnimationStep* AnimationSystem::getCurrentStep() const {
    if (currentStep >= 0 && currentStep < static_cast<int>(steps.size())) {
        return &steps[currentStep];
    }
    return nullptr;
}

void AnimationSystem::onTimerTick() {
    if (!isPlaying || currentStep < 0 || currentStep >= static_cast<int>(steps.size())) {
        return;
    }
    
    const AnimationStep& step = steps[currentStep];
    
    // Increment elapsed time based on speed
    elapsedTime += 16 * animationSpeed;
    
    // Emit update for smooth animation
    emit animationUpdate();
    
    // Check if current step is complete
    if (elapsedTime >= step.duration) {
        // Execute callback if present
        if (step.callback) {
            step.callback();
        }
        
        nextStep();
    }
}

void AnimationSystem::nextStep() {
    currentStep++;
    elapsedTime = 0;
    
    if (currentStep >= static_cast<int>(steps.size())) {
        // Animation complete
        stop();
        emit animationCompleted();
    } else {
        emit animationStepChanged(currentStep);
    }
}
