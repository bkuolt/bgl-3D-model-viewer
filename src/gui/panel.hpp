#pragma once

/**
 * @file panel.hpp
 * @brief GUI panels for model information statistics and render settings controls.
 */

#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

#include "gfx/gl.hpp"

namespace bgl {

enum class RenderMode { Solid, Textured, Wireframe };

class SettingsPanel : public QFrame {
    Q_OBJECT

  public:
    explicit SettingsPanel(QWidget* parent = nullptr);
    ~SettingsPanel() override = default;

    SettingsPanel(SettingsPanel&&) noexcept = default;
    SettingsPanel& operator=(SettingsPanel&&) noexcept = default;

    SettingsPanel(const SettingsPanel&) = delete;
    SettingsPanel& operator=(const SettingsPanel&) = delete;

    [[nodiscard]] RenderMode getRenderMode() const noexcept;
    [[nodiscard]] bool isLightingEnabled() const noexcept;
    [[nodiscard]] bool isBoundingBoxEnabled() const noexcept;

  private:
    QComboBox _renderModeBox;
    QCheckBox _lightingBox;
    QCheckBox _boundingBoxBox;
    QVBoxLayout _layout;
};

class StatisticsPanel : public QFrame {
    Q_OBJECT

  public:
    explicit StatisticsPanel(QWidget* parent = nullptr);
    ~StatisticsPanel() override = default;

    StatisticsPanel(StatisticsPanel&&) noexcept = default;
    StatisticsPanel& operator=(StatisticsPanel&&) noexcept = default;

    StatisticsPanel(const StatisticsPanel&) = delete;
    StatisticsPanel& operator=(const StatisticsPanel&) = delete;

    void updateStatistics(std::size_t vertices, std::size_t triangles, std::size_t materials);

  private:
    QLabel _verticesLabel;
    QLabel _trianglesLabel;
    QLabel _materialsLabel;
    QVBoxLayout _layout;
};

class Panel : public QFrame {
    Q_OBJECT

  public:
    explicit Panel(QWidget* parent = nullptr);
    ~Panel() override = default;

    Panel(Panel&&) noexcept = default;
    Panel& operator=(Panel&&) noexcept = default;

    Panel(const Panel&) = delete;
    Panel& operator=(const Panel&) = delete;

    [[nodiscard]] StatisticsPanel& getStatisticsPanel() noexcept {
        return _statistics;
    }
    [[nodiscard]] SettingsPanel& getSettingsPanel() noexcept {
        return _settings;
    }

  private:
    StatisticsPanel _statistics;
    SettingsPanel _settings;
    QVBoxLayout _layout;
};

}  // namespace bgl
