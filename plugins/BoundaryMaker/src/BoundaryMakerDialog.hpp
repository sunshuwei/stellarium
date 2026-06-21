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

#ifndef BOUNDARYMAKERDIALOG_HPP
#define BOUNDARYMAKERDIALOG_HPP

#include "StelDialog.hpp"
#include <QList>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>

class BoundaryMaker;

struct BoundaryPoint {
	double ra;      // Right Ascension in hours
	double dec;     // Declination in degrees
};

struct BoundaryLine {
	BoundaryPoint start;
	BoundaryPoint end;
	QString constellation1;  // Constellation on one side
	QString constellation2;  // Constellation on the other side
	QString type;            // "M+" for meridian, "M-" for anti-meridian, "P+" for parallel increasing, "P-" for parallel decreasing
};

class BoundaryMakerDialog : public StelDialog
{
	Q_OBJECT
public:
	BoundaryMakerDialog(BoundaryMaker* plugin);
	~BoundaryMakerDialog() override;

public slots:
	void retranslate() override;

protected:
	void createDialogContent() override;

private slots:
	void addPoint();
	void removePoint();
	void clearAll();
	void updateOutput(int = 0);
	void copyToClipboard();
	void saveToFile();
	void updateType(int index);
	void onCanvasClick();
	void onSelectModeChanged(bool enabled);

private:
	void setupUI();
	QString formatBoundaryLine(const BoundaryLine& line) const;
	QString formatCoordinates(double ra, double dec) const;
	
	BoundaryMaker* plugin;
	
	// UI elements
	QComboBox* cboConstellation1;
	QComboBox* cboConstellation2;
	QComboBox* cboLineType;
	QSpinBox* spnDecimals;
	QLineEdit* edtEpoch;
	QDoubleSpinBox* spnRA1;
	QDoubleSpinBox* spnDec1;
	QDoubleSpinBox* spnRA2;
	QDoubleSpinBox* spnDec2;
	QTextEdit* txtOutput;
	QPushButton* btnAddPoint;
	QPushButton* btnRemovePoint;
	QPushButton* btnClearAll;
	QPushButton* btnCopy;
	QPushButton* btnSave;
	QPushButton* btnSelectPoint;
	
	// State
	QList<BoundaryLine> boundaryLines;
	bool selectMode;
};

#endif /* BOUNDARYMAKERDIALOG_HPP */