#include "TriageBridge.hpp"
#include "FibonacciHeap.tpp"
#include <cstdlib>
#include <ctime>
#include <QDebug>

TriageBridge::TriageBridge(QObject* parent)
    : QObject(parent), nextPatientId(1) {
    srand(static_cast<unsigned>(time(nullptr)));  // Seed for randomization
}

int TriageBridge::patientCount() const {
    return heap.getSize();
}

QVariantList TriageBridge::allPatients() const {
    QVariantList list;
    for (const auto& node : heap.getRootList()) {
        list.append(patientToVariant(node->value));
    }
    return list;
}

void TriageBridge::addPatient(QString name, QString condition, int priority) {
    auto patient = std::make_shared<Patient>(nextPatientId++, name, condition, priority);
    patient->severity = getSeverity(priority);
    generateVitals(*patient);  // Generate dummy vitals (heart rate, BP, etc.)
    heap.insert(patient, priority);

    emit patientsChanged();
    emit patientCountChanged();
    emit criticalCountChanged();
    emit urgentCountChanged();
    emit topPatientChanged();
}

void TriageBridge::simulateMassEmergency() {
    for (int i = 0; i < 50; ++i) {
        addPatient(getRandomName(), getRandomCondition(), rand() % 10 + 1);
    }
}

QString TriageBridge::getSeverity(int priority) {
    if (priority <= 1) return "CRITICAL";
    if (priority <= 3) return "URGENT";
    if (priority <= 5) return "MODERATE";
    if (priority <= 8) return "STABLE";
    return "ROUTINE";
}

QVariantMap TriageBridge::patientToVariant(const std::shared_ptr<Patient>& p) const {
    QVariantMap map;
    map["id"] = p->id;
    map["name"] = p->name;
    map["condition"] = p->condition;
    map["priority"] = p->priority;
    map["heartRate"] = p->heartRate;
    map["bloodPressure"] = p->bloodPressure;
    map["temperature"] = p->temperature;
    map["severity"] = p->severity;
    return map;
}

QString TriageBridge::getRandomName() {
    QStringList names = {"John Doe", "Jane Smith", "Alice Green", "Bob Brown", "Charlie White"};
    return names[rand() % names.size()];
}

QString TriageBridge::getRandomCondition() {
    QStringList conditions = {
        "Chest Pain", "Fever", "Fracture", "Severe Allergy", "Shortness of Breath"
    };
    return conditions[rand() % conditions.size()];
}

int TriageBridge::criticalCount() const {
    int count = 0;
    for (const auto& node : heap.getRootList()) {
        if (node->value->priority <= 1) count++;
    }
    return count;
}

int TriageBridge::urgentCount() const {
    int count = 0;
    for (const auto& node : heap.getRootList()) {
        if (node->value->priority > 1 && node->value->priority <= 3) count++;
    }
    return count;
}

int TriageBridge::treatedCount() const {
    return treated;
}

QString TriageBridge::topPatientName() const {
    if (heap.isEmpty()) return "No Patients";
    return heap.getMin()->value->name;
}

QVariantMap TriageBridge::topPatient() const {
    QVariantMap map;
    if (heap.isEmpty()) return map;
    auto minNode = heap.getMin();
    return patientToVariant(minNode->value);
}

void TriageBridge::treatNext() {
    auto minNode = heap.extractMin();
    if (minNode) {
        treated++;
        emit treatedCountChanged();
        emit patientCountChanged();
        emit criticalCountChanged();
        emit urgentCountChanged();
        emit topPatientChanged();
        emit patientsChanged();
        delete minNode;  // Free memory for the treated patient node
    }
}

void TriageBridge::updatePriority(int patientId, int newPriority) {
    auto nodes = heap.getRootList();
    for (auto node : nodes) {
        if (node->value->id == patientId) {
            heap.updateKey(node, newPriority);
            node->value->priority = newPriority;
            node->value->severity = getSeverity(newPriority);
            emit patientsChanged();
            emit criticalCountChanged();
            emit urgentCountChanged();
            emit topPatientChanged();
            return;
        }
    }
    qDebug() << "Patient ID not found!";
}

void TriageBridge::initializeSampleData() {
    addPatient("Sarah Johnson", "Severe chest pain", 2);
    addPatient("David Wilson", "Possible fracture", 4);
    addPatient("Michael Brown", "Head trauma", 6);
    addPatient("Robert Garcia", "Vomiting", 7);
    addPatient("Emily Thompson", "Abdominal pain", 9);
}

void TriageBridge::generateVitals(Patient& p) {
    p.heartRate = rand() % 40 + 60;  // Random heart rate between 60-100 bpm
    p.bloodPressure = QString::number(rand() % 40 + 100) + "/" + QString::number(rand() % 20 + 60);
    p.temperature = (rand() % 30 + 970) / 10.0;  // Random temperature from 97.0°F to 100°F
}
