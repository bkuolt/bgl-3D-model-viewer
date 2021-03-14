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
    void SettingsPanel()
        : _renderMode { "Render Mode:" },
          _lighting { "Lighting" },
          _boundingBox { "Bounding Box" } {
        _renderMode->addItem("Solid");
        _renderMode->addItem("Textured");
        _renderMode->addItem("Wireframe");
        connect(_renderMode, &QComboBox::currentTextChanged, this, &onSettingsChanged);
        connect(_lighting, &QCheckBox::stateChanged, this, &onSettingsChanged);
        connect(_boundingBox, &QCheckBox::stateChanged, this, &onSettingsChanged);

        this->setLayout(&layout);
    }
    
    virtual void onSettingsChanged() {
        // TODO(bkuol)
    }

    enum class RenderMode { /* TODO(bkuolt */ };

    RenderMode renderMode() {
        return {};  // TODO(Vkuolt)
    }

    bool isLit() {
        // TODO(bkuolt)
    }

    bool showBoundingBox() {
        return {};  // TODO(bkuolt)
    }

 private:
    QComboBox _renderMode;
    QCheckBox _lightingMode;
    QCheckBox _boundingBox;
    QLayout _layout;  // todo
};


class StatisticsPanel : public QFrame {
 public:
    StatisticsPanel()
        : _statistics { "Vertices", "Triangles", "Materials" } {
        _layout { new QVBoxLayout();
        _layout.addWidget(&_statistics.triangles);
        _layout.addWidget(&_statistics.vertices);
        _layout.addWidget(&_statistics.materials);
        _layout.addStretch();
        this->setLayout(&layout);
    }

    virtual StatisticsPanel() noexcept = default;

    void setVertexCount(unsigned int num) {
        // TODO(bkuolt)
    }

    void setTriangleCount(unsigned int num) {
        // TODO(bkuolt)
    }

    void setMaterialCount(unsigned int num) {
        // TODO(bkuolt)
    }

 private:
    QVBoxLayout _layout;
    struct {
        QLabel const vertices;
        QLabel const triangles;
        QLabel const materials;
    } _statistics;
}; 


class Panel {
 public:

#if 0
    Panel()
        : _layout { new QVBoxLayout() } {
        setLayout(_layout);
        _layout->addStretch();
        _layout->addWidget(new InfoPanel);
        _layout->addWidget(new ControlPanel);
    }
#endif  // 0

    // Layout
    // Panels
};

}  // namespace bgl

#endif  // GUI_PANEL_HPP_