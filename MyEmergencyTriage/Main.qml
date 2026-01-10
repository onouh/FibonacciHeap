import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 1400
    height: 900
    minimumWidth: 800
    minimumHeight: 600
    title: "Emergency Room Queue - Medical Command Center"
    color: "#f5f7fa"

    // Helper function to get urgency label from priority
    function getUrgencyLabel(priority) {
        if (priority <= 1) return "CRITICAL"
        if (priority <= 3) return "URGENT"
        if (priority <= 5) return "MODERATE"
        if (priority <= 8) return "STABLE"
        return "ROUTINE"
    }

    // Helper function to get urgency color
    function getUrgencyColor(priority) {
        if (priority <= 1) return "#ef4444"  // CRITICAL - red
        if (priority <= 3) return "#f59e0b"  // URGENT - orange
        if (priority <= 5) return "#3b82f6"  // MODERATE - blue
        if (priority <= 8) return "#22c55e"  // STABLE - green
        return "#10b981"                      // ROUTINE - green
    }

    // Helper function to get urgency border color
    function getUrgencyBorderColor(priority) {
        if (priority <= 1) return "#fca5a5"
        if (priority <= 3) return "#fde047"
        if (priority <= 5) return "#93c5fd"
        if (priority <= 8) return "#86efac"
        return "#6ee7b7"
    }

    // Helper function to get urgency text color
    function getUrgencyTextColor(priority) {
        if (priority <= 1) return "#dc2626"
        if (priority <= 3) return "#d97706"
        if (priority <= 5) return "#2563eb"
        if (priority <= 8) return "#16a34a"
        return "#059669"
    }

    // Top Header Bar
    Rectangle {
        id: header
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 80
        color: "white"

        RowLayout {
            anchors {
                fill: parent
                margins: 20
            }
            spacing: 20

            // Logo and Title
            RowLayout {
                spacing: 12

                Rectangle {
                    Layout.preferredWidth: 48
                    Layout.preferredHeight: 48
                    radius: 12
                    color: "#2563eb"

                    Text {
                        anchors.centerIn: parent
                        text: "\u{1F3E5}"
                        font.pixelSize: 28
                    }
                }

                ColumnLayout {
                    spacing: 2

                    Text {
                        text: "ER Triage"
                        font.pixelSize: 24
                        font.weight: Font.Bold
                        color: "#1e293b"
                    }

                    Text {
                        text: "Emergency Management"
                        font.pixelSize: 13
                        color: "#64748b"
                    }
                }
            }

            Item { Layout.fillWidth: true }

            // Digital Clock
            Rectangle {
                Layout.preferredWidth: 180
                Layout.preferredHeight: 50
                radius: 10
                color: "#f1f5f9"

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 8

                    Text {
                        text: "\u{1F550}"
                        font.pixelSize: 20
                    }

                    Text {
                        id: clockText
                        text: Qt.formatDateTime(new Date(), "hh:mm:ss AP")
                        font.pixelSize: 16
                        font.weight: Font.Medium
                        color: "#334155"

                        Timer {
                            interval: 1000
                            running: true
                            repeat: true
                            onTriggered: clockText.text = Qt.formatDateTime(new Date(), "hh:mm:ss AP")
                        }
                    }
                }
            }

            // Admit New Patient Button
            Button {
                Layout.preferredWidth: 180
                Layout.preferredHeight: 50

                background: Rectangle {
                    radius: 10
                    color: parent.hovered ? "#1d4ed8" : "#2563eb"

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 8

                    Text {
                        text: "+"
                        font.pixelSize: 24
                        font.weight: Font.Bold
                        color: "white"
                    }

                    Text {
                        text: "Admit New Patient"
                        font.pixelSize: 14
                        font.weight: Font.Medium
                        color: "white"
                    }
                }

                onClicked: admitDialog.open()
            }
        }

        Rectangle {
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            height: 1
            color: "#e2e8f0"
        }
    }

    // Main Content Area
    ScrollView {
        id: mainScrollView
        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        ColumnLayout {
            id: mainColumn
            width: mainScrollView.availableWidth
            spacing: 24

            // Stats Cards Row
            Flow {
                Layout.fillWidth: true
                Layout.leftMargin: 24
                Layout.rightMargin: 24
                Layout.topMargin: 24
                spacing: 16

                // Total Pending Card
                Rectangle {
                    width: Math.max(690, (mainColumn.width - 48 - 64) / 2 * 1.5)
                    height: 189
                    radius: 12
                    color: "#eff6ff"
                    border.width: 2
                    border.color: "#bfdbfe"

                    ColumnLayout {
                        anchors {
                            fill: parent
                            margins: 20
                        }
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: "Total Pending"
                                font.pixelSize: 14
                                color: "#3b82f6"
                                font.weight: Font.Medium
                            }

                            Item { Layout.fillWidth: true }

                            Text {
                                text: "\u{1F465}"
                                font.pixelSize: 24
                            }
                        }

                        Text {
                            text: TriageSystem ? TriageSystem.patientCount : 0
                            font.pixelSize: 48
                            font.weight: Font.Bold
                            color: "#1e40af"
                        }

                        Text {
                            text: "Patients in queue"
                            font.pixelSize: 12
                            color: "#60a5fa"
                        }
                    }
                }

                // Treated Today Card
                Rectangle {
                    width: Math.max(690, (mainColumn.width - 48 - 64) / 2 * 1.5)
                    height: 189
                    radius: 12
                    color: "#f0fdf4"
                    border.width: 2
                    border.color: "#bbf7d0"

                    ColumnLayout {
                        anchors {
                            fill: parent
                            margins: 20
                        }
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: "Treated Today"
                                font.pixelSize: 14
                                color: "#22c55e"
                                font.weight: Font.Medium
                            }

                            Item { Layout.fillWidth: true }

                            Text {
                                text: "\u{2705}"
                                font.pixelSize: 24
                            }
                        }

                        Text {
                            text: TriageSystem ? TriageSystem.treatedCount : 0
                            font.pixelSize: 48
                            font.weight: Font.Bold
                            color: "#16a34a"
                        }

                        Text {
                            text: "Completed treatments"
                            font.pixelSize: 12
                            color: "#4ade80"
                        }
                    }
                }
            }

            // Emergency Mode Button
            Button {
                id: emergencyButton
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 280
                Layout.preferredHeight: 56

                background: Rectangle {
                    radius: 12
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: emergencyButton.hovered ? "#dc2626" : "#ef4444" }
                        GradientStop { position: 1.0; color: emergencyButton.hovered ? "#b91c1c" : "#dc2626" }
                    }
                }

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 12

                    Text {
                        text: "\u{1F6A8}"
                        font.pixelSize: 24
                    }

                    Text {
                        text: "MASS EMERGENCY MODE"
                        font.pixelSize: 15
                        font.weight: Font.Bold
                        color: "white"
                        font.letterSpacing: 1
                    }
                }

                onClicked: {
                    if (TriageSystem) {
                        TriageSystem.simulateMassEmergency()
                    }
                }
            }

            // Priority Treatment Section
            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 24
                Layout.rightMargin: 24
                spacing: 16

                Text {
                    text: "Priority Treatment"
                    font.pixelSize: 22
                    font.weight: Font.Bold
                    color: "#1e293b"
                }

                // Next Patient Card
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 406
                    radius: 16
                    color: (TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                           getUrgencyColor(TriageSystem.topPatient.priority) : "#94a3b8"

                    ColumnLayout {
                        anchors {
                            fill: parent
                            margins: 24
                        }
                        spacing: 16

                        RowLayout {
                            Layout.fillWidth: true

                            RowLayout {
                                spacing: 12

                                Text {
                                    text: "\u{26A0}\u{FE0F}"
                                    font.pixelSize: 20
                                }

                                Text {
                                    text: (TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                                          getUrgencyLabel(TriageSystem.topPatient.priority) : "READY"
                                    font.pixelSize: 14
                                    font.weight: Font.Bold
                                    color: "white"
                                    font.letterSpacing: 1
                                }
                            }

                            Item { Layout.fillWidth: true }

                            Rectangle {
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 32
                                radius: 8
                                color: Qt.darker((TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                                                 getUrgencyColor(TriageSystem.topPatient.priority) : "#94a3b8", 1.2)
                                visible: TriageSystem && TriageSystem.patientCount > 0

                                Text {
                                    anchors.centerIn: parent
                                    text: "Priority: " + ((TriageSystem && TriageSystem.topPatient) ?
                                          TriageSystem.topPatient.priority : 1)
                                    font.pixelSize: 13
                                    color: "white"
                                    font.weight: Font.Medium
                                }
                            }
                        }

                        Text {
                            text: "Next for Treatment"
                            font.pixelSize: 16
                            color: Qt.lighter((TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                                              getUrgencyColor(TriageSystem.topPatient.priority) : "#94a3b8", 1.8)
                            font.weight: Font.Medium
                        }

                        Text {
                            text: (TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                                  TriageSystem.topPatient.name : "No patients waiting"
                            font.pixelSize: 28
                            font.weight: Font.Bold
                            color: "white"
                        }

                        Text {
                            text: (TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                                  TriageSystem.topPatient.condition : "Ready for next patient admission"
                            font.pixelSize: 14
                            color: Qt.lighter((TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                                              getUrgencyColor(TriageSystem.topPatient.priority) : "#94a3b8", 1.8)
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }

                        RowLayout {
                            spacing: 20
                            visible: TriageSystem && TriageSystem.patientCount > 0

                            Text {
                                text: "\u{2764}\u{FE0F} " + ((TriageSystem && TriageSystem.topPatient) ?
                                      TriageSystem.topPatient.heartRate : 0) + " bpm"
                                font.pixelSize: 13
                                color: "white"
                                font.weight: Font.Medium
                            }

                            Text {
                                text: "\u{1FA7A} BP: " + ((TriageSystem && TriageSystem.topPatient) ?
                                      TriageSystem.topPatient.bloodPressure : "--/--")
                                font.pixelSize: 13
                                color: "white"
                                font.weight: Font.Medium
                            }

                            Text {
                                text: "\u{1F321}\u{FE0F} " + ((TriageSystem && TriageSystem.topPatient) ?
                                      TriageSystem.topPatient.temperature.toFixed(1) : "--") + "\u00B0F"
                                font.pixelSize: 13
                                color: "white"
                                font.weight: Font.Medium
                            }
                        }

                        Item { Layout.fillHeight: true }

                        Button {
                            id: treatmentButton
                            Layout.fillWidth: true
                            Layout.preferredHeight: 48

                            background: Rectangle {
                                radius: 10
                                color: treatmentButton.hovered ? "#ffffff" : "#fef2f2"

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }

                            RowLayout {
                                anchors.centerIn: parent
                                spacing: 10

                                Item { Layout.fillWidth: true }

                                Text {
                                    text: "Start Treatment"
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    color: (TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                                           getUrgencyColor(TriageSystem.topPatient.priority) : "#94a3b8"
                                }

                                Text {
                                    text: "\u2192"
                                    font.pixelSize: 18
                                    font.weight: Font.Bold
                                    color: (TriageSystem && TriageSystem.topPatient && TriageSystem.patientCount > 0) ?
                                           getUrgencyColor(TriageSystem.topPatient.priority) : "#94a3b8"
                                }

                                Item { Layout.fillWidth: true }
                            }

                            enabled: TriageSystem && TriageSystem.patientCount > 0
                            opacity: enabled ? 1.0 : 0.5

                            onClicked: {
                                if (TriageSystem) {
                                    TriageSystem.treatNext()
                                }
                            }
                        }
                    }
                }
            }

            // Live Triage Feed
            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 24
                Layout.rightMargin: 24
                Layout.bottomMargin: 40
                spacing: 16

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "Live Triage Feed"
                        font.pixelSize: 22
                        font.weight: Font.Bold
                        color: "#1e293b"
                    }

                    Item { Layout.fillWidth: true }

                    Rectangle {
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 32
                        radius: 8
                        color: "#f1f5f9"

                        RowLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            Rectangle {
                                Layout.preferredWidth: 8
                                Layout.preferredHeight: 8
                                radius: 4
                                color: "#22c55e"

                                SequentialAnimation on opacity {
                                    loops: Animation.Infinite
                                    NumberAnimation { to: 0.3; duration: 800 }
                                    NumberAnimation { to: 1.0; duration: 800 }
                                }
                            }

                            Text {
                                text: "Live Updates"
                                font.pixelSize: 12
                                color: "#64748b"
                                font.weight: Font.Medium
                            }
                        }
                    }
                }

                // Patient Cards Flow Layout
                Flow {
                    Layout.fillWidth: true
                    spacing: 16

                    Repeater {
                        model: TriageSystem ? TriageSystem.allPatients : []

                        Rectangle {
                            width: Math.max(594, (mainColumn.width - 48 - 16) / 2 * 1.981)
                            height: 195
                            radius: 12
                            color: "white"
                            border.width: 2
                            border.color: getUrgencyBorderColor(modelData.priority)

                            Behavior on border.color {
                                ColorAnimation { duration: 300 }
                            }

                            ColumnLayout {
                                anchors {
                                    fill: parent
                                    margins: 20
                                }
                                spacing: 10

                                RowLayout {
                                    Layout.fillWidth: true

                                    Rectangle {
                                        Layout.preferredWidth: 12
                                        Layout.preferredHeight: 12
                                        radius: 6
                                        color: getUrgencyColor(modelData.priority)
                                    }

                                    Text {
                                        text: getUrgencyLabel(modelData.priority)
                                        font.pixelSize: 11
                                        font.weight: Font.Bold
                                        color: getUrgencyTextColor(modelData.priority)
                                        font.letterSpacing: 0.5
                                    }

                                    Item { Layout.fillWidth: true }

                                    Rectangle {
                                        Layout.preferredWidth: 70
                                        Layout.preferredHeight: 24
                                        radius: 6
                                        color: "#f8fafc"

                                        Text {
                                            anchors.centerIn: parent
                                            text: "P: " + modelData.priority
                                            font.pixelSize: 11
                                            color: "#64748b"
                                            font.weight: Font.Medium
                                        }
                                    }
                                }

                                Text {
                                    text: modelData.name
                                    font.pixelSize: 18
                                    font.weight: Font.Bold
                                    color: "#1e293b"
                                }

                                Text {
                                    text: modelData.condition
                                    font.pixelSize: 12
                                    color: "#64748b"
                                    wrapMode: Text.WordWrap
                                    Layout.fillWidth: true
                                    maximumLineCount: 2
                                    elide: Text.ElideRight
                                }

                                RowLayout {
                                    spacing: 16

                                    Text {
                                        text: "\u{2764}\u{FE0F} " + modelData.heartRate
                                        font.pixelSize: 11
                                        color: "#475569"
                                    }

                                    Text {
                                        text: "BP: " + modelData.bloodPressure
                                        font.pixelSize: 11
                                        color: "#475569"
                                    }

                                    Text {
                                        text: modelData.temperature.toFixed(1) + "\u00B0F"
                                        font.pixelSize: 11
                                        color: "#475569"
                                    }
                                }

                                Item { Layout.fillHeight: true }

                                Button {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 32

                                    background: Rectangle {
                                        radius: 8
                                        color: parent.hovered ? "#f1f5f9" : "#f8fafc"
                                        border.width: 1
                                        border.color: "#cbd5e1"

                                        Behavior on color {
                                            ColorAnimation { duration: 150 }
                                        }
                                    }

                                    Text {
                                        anchors.centerIn: parent
                                        text: "Update Priority"
                                        font.pixelSize: 12
                                        font.weight: Font.Medium
                                        color: "#475569"
                                    }

                                    onClicked: {
                                        updateDialog.patientId = modelData.id
                                        updateDialog.patientName = modelData.name
                                        updateDialog.currentPriority = modelData.priority
                                        updateDialog.open()
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Bottom spacer
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 20
            }
        }
    }

    // Admit New Patient Dialog
    Dialog {
        id: admitDialog
        title: "Admit New Patient"
        modal: true
        anchors.centerIn: parent
        width: 480

        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            spacing: 16
            anchors.fill: parent

            TextField {
                id: nameField
                Layout.fillWidth: true
                placeholderText: "Patient Name"
                font.pixelSize: 14
            }

            TextField {
                id: conditionField
                Layout.fillWidth: true
                placeholderText: "Condition/Symptoms"
                font.pixelSize: 14
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "Priority (1-10):"
                    font.pixelSize: 14
                }

                SpinBox {
                    id: prioritySpinBox
                    from: 1
                    to: 10
                    value: 5
                    editable: true
                }

                Item { Layout.fillWidth: true }

                ColumnLayout {
                    spacing: 4

                    Text {
                        text: "1 = Critical"
                        font.pixelSize: 10
                        color: "#ef4444"
                    }
                    Text {
                        text: "3 = Urgent"
                        font.pixelSize: 10
                        color: "#f59e0b"
                    }
                    Text {
                        text: "5 = Moderate"
                        font.pixelSize: 10
                        color: "#3b82f6"
                    }
                    Text {
                        text: "8 = Stable"
                        font.pixelSize: 10
                        color: "#22c55e"
                    }
                    Text {
                        text: "10 = Routine"
                        font.pixelSize: 10
                        color: "#10b981"
                    }
                }
            }
        }

        onAccepted: {
            if (nameField.text && conditionField.text && TriageSystem) {
                TriageSystem.addPatient(nameField.text, conditionField.text, prioritySpinBox.value)
                nameField.text = ""
                conditionField.text = ""
                prioritySpinBox.value = 5
            }
        }
    }

    // Update Priority Dialog
    Dialog {
        id: updateDialog
        title: "Update Patient Priority"
        modal: true
        anchors.centerIn: parent
        width: 400

        property int patientId: 0
        property string patientName: ""
        property int currentPriority: 0

        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            spacing: 16
            anchors.fill: parent

            Text {
                text: "Patient: " + updateDialog.patientName
                font.pixelSize: 16
                font.weight: Font.Bold
            }

            Text {
                text: "Current Priority: " + updateDialog.currentPriority +
                      " (" + getUrgencyLabel(updateDialog.currentPriority) + ")"
                font.pixelSize: 14
                color: "#64748b"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "New Priority:"
                    font.pixelSize: 14
                }

                SpinBox {
                    id: newPrioritySpinBox
                    from: 1
                    to: 10
                    value: 5
                    editable: true
                }

                Item { Layout.fillWidth: true }
            }

            ColumnLayout {
                spacing: 4
                Layout.fillWidth: true

                Text {
                    text: "Priority Scale:"
                    font.pixelSize: 12
                    font.weight: Font.Bold
                    color: "#475569"
                }
                Text {
                    text: "1 = Critical | 3 = Urgent | 5 = Moderate"
                    font.pixelSize: 11
                    color: "#64748b"
                }
                Text {
                    text: "8 = Stable | 10 = Routine"
                    font.pixelSize: 11
                    color: "#64748b"
                }
            }

            Text {
                text: "Note: Lower values = higher priority"
                font.pixelSize: 12
                color: "#94a3b8"
                font.italic: true
            }
        }

        onAccepted: {
            if (TriageSystem) {
                TriageSystem.updatePriority(patientId, newPrioritySpinBox.value)
            }
        }
    }
}
