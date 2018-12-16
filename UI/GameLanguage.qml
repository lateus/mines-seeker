import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Menu {
    id: menuLanguage

    property color iconColor: "transparent"
    property color materialTextColor: Material.foreground

    MenuItem {
        text: "English"
        icon.source: "qrc:/languages/Languages/en.svg"
        icon.color: menuLanguage.iconColor
        Material.foreground: settings.language === "en" ? Material.accent : menuLanguage.materialTextColor
        onClicked: {
            settings.language = "en"
        }
    }
    MenuItem {
        text: "Español"
        icon.source: "qrc:/languages/Languages/es.svg"
        icon.color: menuLanguage.iconColor
        Material.foreground: settings.language === "es" ? Material.accent : menuLanguage.materialTextColor
        onClicked: {
            settings.language = "es"
        }
    }
}
