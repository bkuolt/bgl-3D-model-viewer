// Copyright 2021 Bastian Kuolt
#ifndef GUI_PANEL_HPP_
#define GUI_PANEL_HPP_

#include <QFrame>
#include <QCheckBox>
#include <QComboBox>
#include <QLayout>
#include <QLabel>


namespace bgl {

class SettingsPanel : public QFrame {
 public:
    enum class RenderMode { /* TODO(bkuolt */ };
    enum class Settings { /* TODO(bkuolt */  };

    SettingsPanel();
    virtual ~SettingsPanel() noexcept = default;

    bool is_set(Settings settings);
    RenderMode get_mode();

 protected:
    virtual void onSettingsChanged();

 private:
    QComboBox _render_mode;
    QCheckBox _lighting;
    QCheckBox _bounding_box;
    QLayout _layout;
};

class StatisticsPanel : public QFrame {
 public:
    enum class StatisticsType { /* TODO */ };

    StatisticsPanel();
    virtual StatisticsPanel() noexcept = default;

    void update(StatisticsType type, unsigned int value);

 private:
    QLabel vertices;
    QLabel triangles;
    QLabel materials;
    QVBoxLayout _layout;
}; 


class Panel : public QPanel {
 public:
    Panel();
    virtual ~Panel()  noexcept = default;
 
 protected:
    virtual void settingsChanged();  // TODO

 private:
    StatisticsPanel _statistics;
    SettingsPanel _settings;
    QVBoxLayout _layout;
};

}  // namespace bgl

#endif  // GUI_PANEL_HPP_