/*
 * Constellation Boundary Maker plug-in for Stellarium
 *
 * Copyright (C) 2025 Your Name
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "BoundaryMakerDialog.hpp"
#include "BoundaryMaker.hpp"
#include "StelApp.hpp"
#include "StelCore.hpp"
#include "StelProjector.hpp"
#include "StelUtils.hpp"
#include "StelTranslator.hpp"
#include "StelGui.hpp"
#include "Dialog.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>
#include <QGuiApplication>

BoundaryMakerDialog::BoundaryMakerDialog(BoundaryMaker* plugin)
	: StelDialog("BoundaryMakerDialog"), plugin(plugin), selectMode(false)
{
}

BoundaryMakerDialog::~BoundaryMakerDialog()
{
}

void BoundaryMakerDialog::retranslate()
{
	// Nothing to retranslate for now
}

void BoundaryMakerDialog::createDialogContent()
{
	setupUI();
	updateOutput();
	
	connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));
}

void BoundaryMakerDialog::setupUI()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	
	// Title bar
	TitleBar* titleBar = new TitleBar(dialog);
	titleBar->setTitle(q_("Constellation Boundary Maker"));
	mainLayout->addWidget(titleBar);
	connect(titleBar, &TitleBar::closeClicked, this, &BoundaryMakerDialog::close);
	connect(titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
	
	// Epoch selection
	QHBoxLayout* epochLayout = new QHBoxLayout();
	QLabel* lblEpoch = new QLabel(q_("Epoch:"));
	edtEpoch = new QLineEdit(plugin->getDefaultEpoch());
	epochLayout->addWidget(lblEpoch);
	epochLayout->addWidget(edtEpoch);
	mainLayout->addLayout(epochLayout);
	
	// Constellation selection
	QHBoxLayout* constellationLayout = new QHBoxLayout();
	QLabel* lblConstellation1 = new QLabel(q_("Constellation 1:"));
	cboConstellation1 = new QComboBox();
	QStringList abbrs = plugin->getConstellationAbbreviations();
	if (abbrs.isEmpty()) {
		abbrs << "AND" << "PEG" << "CAS" << "CEP" << "LYR" << "AQU" << "PIC";
	}
	cboConstellation1->addItems(abbrs);
	QLabel* lblConstellation2 = new QLabel(q_("Constellation 2:"));
	cboConstellation2 = new QComboBox();
	cboConstellation2->addItems(abbrs);
	if (abbrs.size() > 1) {
		cboConstellation2->setCurrentIndex(1);
	}
	constellationLayout->addWidget(lblConstellation1);
	constellationLayout->addWidget(cboConstellation1);
	constellationLayout->addWidget(lblConstellation2);
	constellationLayout->addWidget(cboConstellation2);
	mainLayout->addLayout(constellationLayout);
	
	// Line type selection
	QHBoxLayout* typeLayout = new QHBoxLayout();
	QLabel* lblType = new QLabel(q_("Line Type:"));
	cboLineType = new QComboBox();
	cboLineType->addItem(q_("M+ (Meridian increasing)"), "M+");
	cboLineType->addItem(q_("M- (Meridian decreasing)"), "M-");
	cboLineType->addItem(q_("P+ (Parallel increasing)"), "P+");
	cboLineType->addItem(q_("P- (Parallel decreasing)"), "P-");
	typeLayout->addWidget(lblType);
	typeLayout->addWidget(cboLineType);
	mainLayout->addLayout(typeLayout);
	
	// Coordinate inputs
	QGridLayout* coordLayout = new QGridLayout();
	
	QLabel* lblPoint1 = new QLabel(q_("Point 1:"));
	QLabel* lblRA1 = new QLabel(q_("RA (h):"));
	spnRA1 = new QDoubleSpinBox();
	spnRA1->setRange(0.0, 24.0);
	spnRA1->setDecimals(6);
	spnRA1->setValue(12.0);
	QLabel* lblDec1 = new QLabel(q_("Dec (deg):"));
	spnDec1 = new QDoubleSpinBox();
	spnDec1->setRange(-90.0, 90.0);
	spnDec1->setDecimals(6);
	spnDec1->setValue(0.0);
	
	QLabel* lblPoint2 = new QLabel(q_("Point 2:"));
	QLabel* lblRA2 = new QLabel(q_("RA (h):"));
	spnRA2 = new QDoubleSpinBox();
	spnRA2->setRange(0.0, 24.0);
	spnRA2->setDecimals(6);
	spnRA2->setValue(13.0);
	QLabel* lblDec2 = new QLabel(q_("Dec (deg):"));
	spnDec2 = new QDoubleSpinBox();
	spnDec2->setRange(-90.0, 90.0);
	spnDec2->setDecimals(6);
	spnDec2->setValue(10.0);
	
	coordLayout->addWidget(lblPoint1, 0, 0);
	coordLayout->addWidget(lblRA1, 1, 0);
	coordLayout->addWidget(spnRA1, 1, 1);
	coordLayout->addWidget(lblDec1, 1, 2);
	coordLayout->addWidget(spnDec1, 1, 3);
	
	coordLayout->addWidget(lblPoint2, 2, 0);
	coordLayout->addWidget(lblRA2, 3, 0);
	coordLayout->addWidget(spnRA2, 3, 1);
	coordLayout->addWidget(lblDec2, 3, 2);
	coordLayout->addWidget(spnDec2, 3, 3);
	
	mainLayout->addLayout(coordLayout);
	
	// Decimal places
	QHBoxLayout* decimalLayout = new QHBoxLayout();
	QLabel* lblDecimals = new QLabel(q_("Decimal places:"));
	spnDecimals = new QSpinBox();
	spnDecimals->setRange(0, 6);
	spnDecimals->setValue(2);
	decimalLayout->addWidget(lblDecimals);
	decimalLayout->addWidget(spnDecimals);
	mainLayout->addLayout(decimalLayout);
	
	// Action buttons
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	btnAddPoint = new QPushButton(q_("Add Boundary Line"));
	btnRemovePoint = new QPushButton(q_("Remove Last Line"));
	btnClearAll = new QPushButton(q_("Clear All"));
	btnSelectPoint = new QPushButton(q_("Select on Sky"));
	buttonLayout->addWidget(btnAddPoint);
	buttonLayout->addWidget(btnRemovePoint);
	buttonLayout->addWidget(btnClearAll);
	buttonLayout->addWidget(btnSelectPoint);
	mainLayout->addLayout(buttonLayout);
	
	// Output text area
	txtOutput = new QTextEdit();
	txtOutput->setReadOnly(true);
	txtOutput->setMinimumHeight(200);
	mainLayout->addWidget(txtOutput);
	
	// Copy and Save buttons
	QHBoxLayout* outputButtonLayout = new QHBoxLayout();
	btnCopy = new QPushButton(q_("Copy to Clipboard"));
	btnSave = new QPushButton(q_("Save to File"));
	outputButtonLayout->addWidget(btnCopy);
	outputButtonLayout->addWidget(btnSave);
	mainLayout->addLayout(outputButtonLayout);
	
	// Connect signals
	connect(btnAddPoint, &QPushButton::clicked, this, &BoundaryMakerDialog::addPoint);
	connect(btnRemovePoint, &QPushButton::clicked, this, &BoundaryMakerDialog::removePoint);
	connect(btnClearAll, &QPushButton::clicked, this, &BoundaryMakerDialog::clearAll);
	connect(btnCopy, &QPushButton::clicked, this, &BoundaryMakerDialog::copyToClipboard);
	connect(btnSave, &QPushButton::clicked, this, &BoundaryMakerDialog::saveToFile);
	connect(btnSelectPoint, &QPushButton::toggled, this, &BoundaryMakerDialog::onSelectModeChanged);
	connect(cboLineType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BoundaryMakerDialog::updateType);
	
	// Update output when values change
	connect(spnRA1, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &BoundaryMakerDialog::updateOutput);
	connect(spnDec1, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &BoundaryMakerDialog::updateOutput);
	connect(spnRA2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &BoundaryMakerDialog::updateOutput);
	connect(spnDec2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &BoundaryMakerDialog::updateOutput);
	connect(spnDecimals, QOverload<int>::of(&QSpinBox::valueChanged), this, &BoundaryMakerDialog::updateOutput);
	connect(cboConstellation1, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BoundaryMakerDialog::updateOutput);
	connect(cboConstellation2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BoundaryMakerDialog::updateOutput);
	connect(cboLineType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BoundaryMakerDialog::updateOutput);
}

void BoundaryMakerDialog::addPoint()
{
	BoundaryLine line;
	line.start.ra = spnRA1->value();
	line.start.dec = spnDec1->value();
	line.end.ra = spnRA2->value();
	line.end.dec = spnDec2->value();
	line.constellation1 = cboConstellation1->currentText();
	line.constellation2 = cboConstellation2->currentText();
	line.type = cboLineType->currentData().toString();
	
	boundaryLines.append(line);
	
	// Move point 2 to point 1 for next line
	spnRA1->setValue(spnRA2->value());
	spnDec1->setValue(spnDec2->value());
	
	updateOutput();
}

void BoundaryMakerDialog::removePoint()
{
	if (!boundaryLines.isEmpty())
	{
		boundaryLines.removeLast();
		updateOutput();
	}
}

void BoundaryMakerDialog::clearAll()
{
	boundaryLines.clear();
	updateOutput();
}

void BoundaryMakerDialog::updateOutput(int)
{
	QString output;
	
	// Show currently editing line
	BoundaryLine currentLine;
	currentLine.start.ra = spnRA1->value();
	currentLine.start.dec = spnDec1->value();
	currentLine.end.ra = spnRA2->value();
	currentLine.end.dec = spnDec2->value();
	currentLine.constellation1 = cboConstellation1->currentText();
	currentLine.constellation2 = cboConstellation2->currentText();
	currentLine.type = cboLineType->currentData().toString();
	
	output += "// " + q_("Current line (not added yet):") + "\n";
	output += formatBoundaryLine(currentLine) + "\n\n";
	
	// Show added lines
	output += "// " + q_("Added boundary lines:") + "\n";
	for (const BoundaryLine& line : boundaryLines)
	{
		output += formatBoundaryLine(line) + "\n";
	}
	
	txtOutput->setPlainText(output);
}

void BoundaryMakerDialog::copyToClipboard()
{
	QString output;
	for (const BoundaryLine& line : boundaryLines)
	{
		output += formatBoundaryLine(line) + "\n";
	}
	QGuiApplication::clipboard()->setText(output);
	QMessageBox::information(dialog, q_("Copied"), q_("Boundary lines copied to clipboard!"));
}

void BoundaryMakerDialog::saveToFile()
{
	QString fileName = QFileDialog::getSaveFileName(dialog, q_("Save Boundary Lines"), 
	                                               "boundaries.txt", q_("Text Files (*.txt);;All Files (*)"));
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream out(&file);
			out << "// " << q_("Constellation Boundaries") << "\n";
			out << "// " << q_("Epoch") << ": " << edtEpoch->text() << "\n\n";
			for (const BoundaryLine& line : boundaryLines)
			{
				out << formatBoundaryLine(line) << "\n";
			}
			file.close();
			QMessageBox::information(dialog, q_("Saved"), q_("Boundary lines saved to file!"));
		}
	}
}

void BoundaryMakerDialog::updateType(int index)
{
	Q_UNUSED(index);
	updateOutput();
}

void BoundaryMakerDialog::onSelectModeChanged(bool enabled)
{
	selectMode = enabled;
	if (enabled)
	{
		btnSelectPoint->setText(q_("Cancel Selection"));
		QMessageBox::information(dialog, q_("Selection Mode"), q_("Click on the sky to select coordinates. Click again for second point."));
	}
	else
	{
		btnSelectPoint->setText(q_("Select on Sky"));
	}
}

void BoundaryMakerDialog::onCanvasClick()
{
	if (!selectMode)
		return;
	
	StelCore* core = StelApp::getInstance().getCore();
	// In a real implementation, you would capture mouse position and convert to sky coordinates
	// This is a placeholder - actual implementation would need mouse event handling
	QMessageBox::information(dialog, q_("Info"), q_("Coordinate selection would be implemented here."));
	selectMode = false;
	btnSelectPoint->setChecked(false);
}

QString BoundaryMakerDialog::formatBoundaryLine(const BoundaryLine& line) const
{
	QString ra1 = formatCoordinates(line.start.ra, line.start.dec).split(" ").at(0);
	QString dec1 = formatCoordinates(line.start.ra, line.start.dec).split(" ").at(1);
	QString ra2 = formatCoordinates(line.end.ra, line.end.dec).split(" ").at(0);
	QString dec2 = formatCoordinates(line.end.ra, line.end.dec).split(" ").at(1);
	
	// Format: ___:___ type ra1 dec1 ra2 dec2 const1 const2
	return QString("___:___ %1 %2 %3 %4 %5 %6 %7")
	        .arg(line.type)
	        .arg(ra1)
	        .arg(dec1)
	        .arg(ra2)
	        .arg(dec2)
	        .arg(line.constellation1)
	        .arg(line.constellation2);
}

QString BoundaryMakerDialog::formatCoordinates(double ra, double dec) const
{
	int decimals = spnDecimals->value();
	
	// Format RA in hours:minutes:seconds
	double raHours = ra;
	int raH = static_cast<int>(raHours);
	double raMinutes = (raHours - raH) * 60;
	int raM = static_cast<int>(raMinutes);
	double raSeconds = (raMinutes - raM) * 60;
	
	// Format Dec in degrees:minutes:seconds
	double decDeg = dec;
	int decD = static_cast<int>(decDeg);
	double decMinutes = fabs((decDeg - decD) * 60);
	int decM = static_cast<int>(decMinutes);
	double decSeconds = (decMinutes - decM) * 60;
	
	// Determine sign for declination
	QString decSign = decD >= 0 ? "+" : "";
	
	if (decimals == 0)
	{
		return QString("%1:%2:%3 %4%5:%6:%7")
		        .arg(raH, 2, 10, QChar('0'))
		        .arg(raM, 2, 10, QChar('0'))
		        .arg(static_cast<int>(raSeconds), 2, 10, QChar('0'))
		        .arg(decSign)
		        .arg(qAbs(decD), 2, 10, QChar('0'))
		        .arg(decM, 2, 10, QChar('0'))
		        .arg(static_cast<int>(decSeconds), 2, 10, QChar('0'));
	}
	else
	{
		return QString("%1:%2:%3.%4 %5%6:%7:%8.%9")
		        .arg(raH, 2, 10, QChar('0'))
		        .arg(raM, 2, 10, QChar('0'))
		        .arg(static_cast<int>(raSeconds), 2, 10, QChar('0'))
		        .arg(QString::number(raSeconds - static_cast<int>(raSeconds), 'f', decimals).remove("0."))
		        .arg(decSign)
		        .arg(qAbs(decD), 2, 10, QChar('0'))
		        .arg(decM, 2, 10, QChar('0'))
		        .arg(static_cast<int>(decSeconds), 2, 10, QChar('0'))
		        .arg(QString::number(decSeconds - static_cast<int>(decSeconds), 'f', decimals).remove("0."));
	}
}