#if 0
#include "panel.hpp"

#include <QFrame>
#include <QCheckBox>
#include <QComboBox>
#include <QLayout>
#include <QLabel>

#include <iostream>
#include <sstream>
#include <iosfwd>
#include <iomanip>


namespace bgl {

SettingsPanel::SettingsPanel()
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

void SettingsPanel::onSettingsChanged() {
	// TODO
}

bool SettingsPanel::is_set(Settings settings) {
	const QCheckBox *checkBox { nullptr };
	switch (settings) {
		case 0:
			break;
		default:
			assert(false, "");
	}

	return checkBox->checkState() == Qt::Checked;
}

RenderMode SettingsPanel::get_mode() {
	return {};  // TODO
}


StatisticsPanel::StatisticsPanel()
	: _statistics { "Vertices", "Triangles", "Materials" } {
	_layout { new QVBoxLayout();
	_layout.addWidget(&_statistics.triangles);
	_layout.addWidget(&_statistics.vertices);
	_layout.addWidget(&_statistics.materials);
	_layout.addStretch();
	this->setLayout(&layout);
}

void StatisticsPanel::update(StatisticsType type, unsigned int value) {
	// TODO
}

Panel::Panel() {
	_layout.addStretch();
	_layout.addWidget(&_satistics);
	_layout.addWidget(&_settings);
	this->setLayout(&_layout);
}

namespace {

void onStateChange(QCheckBox * checkbox, int state) {
    std::cout << checkbox->text().toStdString() << " changed to " 
              << (state == Qt::Checked ? "on" : "off")
              << std::endl;
}

// render mode change
void onSelectionChange(QComboBox * comboBox, const QString &text) {
    std::cout << text.toStdString() << " selected" << std::endl;
}

void onTextureModeChanged(QComboBox * comboBox, const QString &text) {
    // TODO:
}

}  // anonymous namespace


ControlPanel::ControlPanel()
    : _comboBox { new QComboBox },
      _checkbox1 { new QCheckBox("Light") },
      _checkbox3 { new QCheckBox("Bounding Box") }
{
    // set rendering modes
    _comboBox->addItem("Solid");
    _comboBox->addItem("Textured");
    _comboBox->addItem("Wireframe");

    // _comboBox->setDisabled(true);

    // TODO: ComboBox Texture Sampling selection ("Nearest", "Linear")

    // set frame layout
    QVBoxLayout * const layout { new QVBoxLayout() };
    setFrameShape(QFrame::HLine);
    setFrameStyle(1);
    setLayout(layout);

    layout->addWidget(_checkbox1);
    layout->addWidget(_checkbox3);
    layout->addWidget(_comboBox);
    layout->addStretch();  // disables stretching

    // TODO: simplify
    connect(_checkbox1, &QCheckBox::stateChanged,
            std::bind(&onStateChange, _checkbox1, std::placeholders::_1));
    connect(_checkbox3, &QCheckBox::stateChanged,
            std::bind(&onStateChange, _checkbox3, std::placeholders::_1));

    connect(_comboBox, &QComboBox::currentTextChanged,
            std::bind(&onSelectionChange, _comboBox, std::placeholders::_1));
}


InfoPanel::InfoPanel(unsigned vertices, unsigned materials, unsigned triangles)
    : _vertices { createLabel("Vertices", vertices) },
        _triangles { createLabel("Triangles", triangles) },
        _materials { createLabel("Materials", materials) } {

    setFrameShape(QFrame::HLine);  // TODO: rethink
    setFrameStyle(1);              // TODO: rethink

    QVBoxLayout * const layout { new QVBoxLayout() };
    layout->addWidget(_triangles);
    layout->addWidget(_vertices);
    layout->addWidget(_materials);
    layout->addStretch();
    setLayout(layout);
}

QLabel* InfoPanel::createLabel(const std::string &text, unsigned number) {
    std::ostringstream oss;
    oss << text << ": " << std::setw(6) << number;
    return new QLabel { oss.str().c_str() };
}


Panel::Panel()
    : _layout { new QVBoxLayout() } {
    setLayout(_layout);
    _layout->addStretch();
    _layout->addWidget(new InfoPanel);
    _layout->addWidget(new ControlPanel);
}

#endif  // 0
