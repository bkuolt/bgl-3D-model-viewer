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
    enum class RenderMode { /* TODO */ };
    enum class Settings { /* TODO */  };

	SettingsPanel();
	SettingsPanel(SettingsPanel&&) = default;
	StatisticsPanel& operator=(SettingsPanel&&) = default;

	SettingsPanel(const SettingsPanel&) = delete;
	SettingsPanel& operator=(const SettingsPanel&) = delete;

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
	StatisticsPanel(StatisticsPanel&&) = default;
	StatisticsPanel& operator=(StatisticsPanel&&) = default;

	StatisticsPanel(const StatisticsPanel&) = delete;
	StatisticsPanel& operator=(const StatisticsPanel&) = delete;

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
	Panel(Panel&&) = default;
	Panel& operator=(Panel&&) = default;

	Panel(const Panel&) = delete;
	Panel& operator=(const Panel&) = delete;

	virtual Panel() noexcept = default;

 protected:
	virtual void settingsChanged();  // TODO

 private:
	StatisticsPanel _statistics;
	SettingsPanel _settings;
	QVBoxLayout _layout;
};

}  // namespace bgl

#endif  // GUI_PANEL_HPP_
