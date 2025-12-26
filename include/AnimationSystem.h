#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <QObject>
#include <QTimer>
#include <QPointF>
#include <QColor>
#include <vector>
#include <functional>

/**
 * Represents a single animation step
 */
struct AnimationStep {
    enum Type {
        MOVE_NODE,
        CHANGE_COLOR,
        HIGHLIGHT_NODE,
        REMOVE_NODE,
        ADD_LINK,
        REMOVE_LINK,
        PAUSE
    };
    
    Type type;
    void* nodePtr;  // Pointer to the node being animated
    QPointF startPos;
    QPointF endPos;
    QColor startColor;
    QColor endColor;
    int duration;  // milliseconds
    std::function<void()> callback;  // Called when step completes
};

/**
 * Animation system for visualizing heap operations
 */
class AnimationSystem : public QObject {
    Q_OBJECT
    
private:
    std::vector<AnimationStep> steps;
    int currentStep;
    QTimer* timer;
    int elapsedTime;
    bool isPlaying;
    int animationSpeed;  // 1 = normal, 2 = fast, 3 = very fast
    
public:
    explicit AnimationSystem(QObject* parent = nullptr);
    ~AnimationSystem();
    
    // Animation control
    void addStep(const AnimationStep& step);
    void clearSteps();
    void play();
    void pause();
    void resume();
    void stop();
    void setSpeed(int speed);  // 1-3
    bool isAnimating() const { return isPlaying; }
    
    // Get current animation progress (0.0 to 1.0)
    float getCurrentProgress() const;
    const AnimationStep* getCurrentStep() const;
    
signals:
    void animationStarted();
    void animationStepChanged(int stepIndex);
    void animationCompleted();
    void animationUpdate();  // Emit during animation for smooth updates
    
private slots:
    void onTimerTick();
    void nextStep();
};

#endif // ANIMATION_SYSTEM_H
