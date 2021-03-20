// Copyright 2021 Bastian Kuolt
#include "panel.hpp"

#include <QFrame>
#include <QCheckBox>
#include <QComboBox>
#include <QLayout>
#include <QLabel>


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
	// TODO(bkuol)
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

}  // namespace bgl
