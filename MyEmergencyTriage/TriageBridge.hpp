#ifndef TRIAGEBRIDGE_HPP
#define TRIAGEBRIDGE_HPP

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include "FibonacciHeap.hpp"
#include "FibonacciHeap.tpp"
#include <memory>
#include <random>

// Custom structure for patient data
struct Patient {
    int id;
    QString name;
    QString condition;
    int priority;
    int heartRate;
    QString bloodPressure;
    double temperature;
    int waitTime;
    QString severity;

    Patient(int _id, QString _name, QString _cond, int _priority)
        : id(_id), name(_name), condition(_cond), priority(_priority),
        heartRate(70), bloodPressure("120/80"), temperature(98.6), waitTime(0),
        severity("STABLE") {}
};

class TriageBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(int patientCount READ patientCount NOTIFY patientCountChanged)
    Q_PROPERTY(QVariantList allPatients READ allPatients NOTIFY patientsChanged)
    Q_PROPERTY(int criticalCount READ criticalCount NOTIFY criticalCountChanged)
    Q_PROPERTY(int urgentCount READ urgentCount NOTIFY urgentCountChanged)
    Q_PROPERTY(int treatedCount READ treatedCount NOTIFY treatedCountChanged)
    Q_PROPERTY(QString topPatientName READ topPatientName NOTIFY topPatientChanged)
    Q_PROPERTY(QVariantMap topPatient READ topPatient NOTIFY topPatientChanged)

public:
    explicit TriageBridge(QObject* parent = nullptr);

    int patientCount() const;
    QVariantList allPatients() const;

    // QML-exposed methods
    Q_INVOKABLE void addPatient(QString name, QString condition, int priority);
    Q_INVOKABLE void simulateMassEmergency();
    Q_INVOKABLE void treatNext();
    Q_INVOKABLE void updatePriority(int patientId, int newPriority);
    Q_INVOKABLE void initializeSampleData();

    // Severity level counters
    int criticalCount() const;
    int urgentCount() const;
    int treatedCount() const;

    QString topPatientName() const;
    QVariantMap topPatient() const;

signals:
    void patientCountChanged();
    void patientsChanged();
    void criticalCountChanged();
    void urgentCountChanged();
    void treatedCountChanged();
    void topPatientChanged();

private:
    FibonacciHeap<std::shared_ptr<Patient>> heap;
    int nextPatientId;
    int treated = 0;

    QVariantMap patientToVariant(const std::shared_ptr<Patient>& p) const;

    // Helper methods
    QString getSeverity(int priority);
    QString getRandomName();
    QString getRandomCondition();
    void generateVitals(Patient& p);
};

#endif // TRIAGEBRIDGE_HPP
