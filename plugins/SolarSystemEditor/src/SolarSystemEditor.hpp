/*
 * Solar System editor plug-in for Stellarium
 * 
 * Copyright (C) 2010 Bogdan Marinov
 * Copyright (C) 2017 Georg Zotti (changes for 0.16)
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#ifndef SOLARSYSTEMEDITOR_HPP
#define SOLARSYSTEMEDITOR_HPP

#include "StelModule.hpp"

#include <QHash>
#include <QList>
#include <QString>
#include <QVariant>

class SolarSystemManagerWindow;
class SolarSystem;
class QSettings;

//! Convenience type for storage of SSO properties in ssystem_minor.ini format.
//! This is an easy way of storing data in the format used in Stellarium's
//! solar system configuration file.
//! What would be key/value pairs in a section in the ssystem_minor.ini file
//! are key/value pairs in the hash. The section name is stored with key
//! "section_name".
//! As it is a hash, key names are not stored alphabetically. This allows
//! for rapid addition and look-up of values, unlike a real QSettings
//! object in StelIniFormat.
//! Also, using this way may allow scripts to define SSOs.
//! \todo Better name.
typedef QHash<QString, QVariant> SsoElements;

typedef QPair<QString, QString> DiscoveryCircumstances;

//! Data which can be used to add extra designations or discovery details to data retrieved from MPC or other sources
typedef struct
{
	QString date_code;       //! date designation (Unique), e.g. "C/2022 A2"
	QString perihelion_code; //! perihelion designation
	QString discovery_code;  //! discovery designation
	QString discovery_date;  //! date of discovery (format: YYYY-MM-DD)
	QString discoverer;      //! name of discoverer
} CometDiscoveryData;

/*!
 \class SolarSystemEditor
 \brief Main class of the Solar System Editor plug-in which allows editing (add, delete, update) of the minor bodies.
 \author Bogdan Marinov, Georg Zotti (0.16 changes)

 Solar System bodies are identified by their names in Stellarium, but entries
 in the configuration file are identified by their group (section) names.
 This makes the detection of duplicates more difficult.
*/
class SolarSystemEditor : public StelModule
{
	Q_OBJECT

public:
	SolarSystemEditor();
	~SolarSystemEditor() override;
	
	///////////////////////////////////////////////////////////////////////////
	// Methods inherited from the StelModule class
	//! called when the plug-in is loaded.
	//! All initializations should be done here.
	void init() override;

	//double getCallOrder(StelModuleActionName actionName) const override;

	//! called when the "configure" button in the "Plugins" tab is pressed
	bool configureGui(bool show) override;
	
	//! Reads a single comet's orbital elements from a string.
	//! This function converts a line of comet orbital elements in MPC format
	//! to a hash in Stellarium's ssystem.ini format.
	//! The MPC's one-line orbital elements format for comets
	//! is described on their website:
	//! http://www.minorplanetcenter.org/iau/info/CometOrbitFormat.html
	//! \returns an empty hash if there is an error or the source string is not
	//! a valid line in MPC format.
	//! \todo Recognise the long form packed designations (to handle fragments)
	//! \todo Handle better any unusual symbols in section names (URL encoding?)
	//! \todo Use column cuts instead of a regular expression?
	SsoElements readMpcOneLineCometElements(const QString &oneLineElements) const;

	//! Reads a single minor planet's orbital elements from a string.
	//! This function converts a line of minor planet orbital elements in
	//! MPC format to a hash in Stellarium's ssystem.ini format.
	//! The MPC's one-line orbital elements format for minor planets
	//! is described on their website:
	//! http://www.minorplanetcenter.org/iau/info/MPOrbitFormat.html
	//! \returns an empty hash if there is an error or the source string is not
	//! a valid line in MPC format.
	//! \todo Handle better any unusual symbols in section names (URL encoding?)
	SsoElements readMpcOneLineMinorPlanetElements(const QString &oneLineElements) const;

	//! Reads a list of comet orbital elements from a file.
	//! This function reads a list of comet orbital elements in MPC's one-line
	//! format from a file (one comet per line) and converts it to a list of
	//! hashes in Stellarium's ssystem.ini format.
	//! Example source file is the list of observable comets on the MPC's site:
	//! http://www.minorplanetcenter.org/iau/Ephemerides/Comets/Soft00Cmt.txt
	//! readMpcOneLineCometElements() is used internally to parse each line.
	QList<SsoElements> readMpcOneLineCometElementsFromFile(const QString &filePath) const;

	//! Reads a list of minor planet orbital elements from a file.
	//! This function reads a list of minor planets orbital elements in MPC's
	//! one-line format from a file (one comet per line) and converts it to
	//! a list of hashes in Stellarium's ssystem.ini format.
	//! Example source file is the list of bright asteroids on the MPC's site:
	//! http://www.minorplanetcenter.org/iau/Ephemerides/Bright/2010/Soft00Bright.txt
	//! readMpcOneLineMinorPlanetElements() is used internally to parse each line.
	QList<SsoElements> readMpcOneLineMinorPlanetElementsFromFile(const QString &filePath) const;

	//! Adds a new entry at the end of the user solar system configuration file.
	//! This function writes directly to the file. See the note on why QSettings
	//! was not used in the description of
	//! appendToSolarSystemConfigurationFile(QList<SsoElements>)
	//! Duplicates are removed: If any section in the file matches the
	//! "section_name" value of the inserted entry, it is removed.
	bool appendToSolarSystemConfigurationFile(const SsoElements &object);

	//! Adds new entries at the end of the user solar system configuration file.
	//! XXX Also updates existing objects (by removing and then appending)
	//! This function writes directly to the file. QSettings was not used, as:
	//!  - Using QSettings with QSettings::IniFormat causes the list in the
	//!    "color" field (e.g. "1.0, 1.0, 1.0") to be wrapped in double quotation
	//!    marks (Stellarium requires no quotation marks).
	//!    (Has been fixed by using StelIniFormat)
	//!  - Using QSettings with StelIniFormat causes unacceptable append times
	//!    when the file grows (>~40 entries). This most probably happens because
	//!    StelIniParser uses QMap internally for the entry list. QMap orders its
	//!    keys (in the case of strings - alphabetically) and it has to find
	//!    the appropriate place in the ordering for every new key, which takes
	//!    more and more time as the list grows.
	//!    (0.16pre: This problem seems to have been solved: I (GZ) have changed
	//!     this plugin to use StelIniParser, and loading 1000comets takes about 4 seconds.
	//!     This is very acceptable, and avoids tons of problems with the default QSettings::IniFormat)
	//!
	//! Duplicates are removed: If any section in the file matches the
	//! "section_name" value of a new entry, it is removed.
	//! Invalid entries in the list (that don't contain a value for
	//! "section_name" or it is an empty string) are skipped and the processing
	//! continues from the next entry.
	//! \todo Protect the default Solar System configuration?
	//! \todo At least warn when overwriting old entries?
	bool appendToSolarSystemConfigurationFile(const QList<SsoElements> &objectList);

	//! Flags to control the updateSolarSystemConfigurationFile() function.
	enum UpdateFlag {
		UpdateNameAndNumber 		= 0x01,	//!< Update the name and minor planet number, if any.
		UpdateType 			= 0x02,	//!< Update objects that lack the "type" parameter.
		UpdateOrbitalElements 		= 0x04,	//!< Update the orbital elements, including the orbit function.
		UpdateMagnitudeParameters 	= 0x08 	//!< Update the values in the two parameter system, or add them if they are missing and the type allows.
	};
	Q_DECLARE_FLAGS(UpdateFlags, UpdateFlag)

	//! Updates entries in the user solar system configuration file.
	//! \param objects a list of data for already existing objects (non-existing ones are skipped);
	//! \param flags flags controlling what is being updated. See UpdateFlag.
	//! \returns false if the operation has failed for some reason.
	bool updateSolarSystemConfigurationFile(const QList<SsoElements> &objects, UpdateFlags flags);

	//! Returns the names of the objects listed in the default ssystem_major.ini.
	//! The default solar system configuration file is assumed to be the one
	//! in the installation directory.
	QHash<QString,QString> getDefaultSsoIdentifiers() const {return defaultSsoIdentifiers;}

	//! Lists the objects listed in the current user ssystem.ini.
	//! As the name suggests, the list is compiled when the function is run.
	QHash<QString,QString> listAllLoadedSsoIdentifiers() const;

	//! Removes an object from the user Solar System configuration file.
	//! Reloads the Solar System on successful removal.
	//! \arg name true name of the object ("name" parameter in the configuration file)
	//! \returns true if the entry has been removed successfully or there is
	//! no such entry
	//! \returns false if there was an error
	bool removeSsoWithName(const QString &name);

	//! Export current minor bodies file from user data directory (if it exists) to filePath. Return true on success.
	bool copySolarSystemConfigurationFileTo(const QString &filePath);

	//! Replace current minor bodies file in the user data directory.
	//! Writes warning to logfile and returns false in case of problems.
	bool replaceSolarSystemConfigurationFileWith(const QString &filePath);

	//! (new 0.16)
	//! Loads all new objects from filePath (an .ini file), and updates existing objects
	//! Default proposal is ssystem_1000comets.ini in the installation dir.
	bool addFromSolarSystemConfigurationFile(const QString &filePath);

	//! returns the path
	QString getCustomSolarSystemFilePath() const {return customSolarSystemFilePath;}

	//! Converts a two-character number used in MPC packed IAU designations.
	//! See http://www.minorplanetcenter.org/iau/info/PackedDes.html
	//! This function is used for both asteroid and comet designations.
	static int unpackAlphanumericNumber (QChar prefix, int lastDigit);

	// Make public slots to allow script-based decoding.
public slots:
	//! Unpacks an MPC packed minor planet IAU designation.
	//! See https://www.minorplanetcenter.org/iau/info/PackedDes.html
	//! \returns an empty string if the argument is not a valid packed
	//! IAU designation.
	static QString unpackMinorPlanetIAUDesignation(const QString &packedDesignation);

	//! Unpacks an MPC packed comet IAU designation. (7-letter variant only)
	//! See https://www.minorplanetcenter.org/iau/info/PackedDes.html
	//! \returns an empty string if the argument is not a valid packed IAU designation.
	static QString unpackCometIAUDesignation(const QString &packedDesignation);

	//! Resets the Solar System configuration file and reloads the Solar System.
	//! \todo Return a bool and make the GUI display a message if it was not successful.
	void resetSolarSystemToDefault();

signals:
	//TODO: This should be part of SolarSystem::reloadPlanets()
	void solarSystemChanged();

private slots:
	void updateI18n();

private:
	bool isInitialized;

	//! Main window of the module's GUI
	SolarSystemManagerWindow * mainWindow;

	QSettings * solarSystemConfigurationFile;
	SolarSystem * solarSystem;

	QString customSolarSystemFilePath;
	QString defaultSolarSystemFilePath;
	QString majorSolarSystemFilePath; // install dir's ssystem_major.ini

	//! A hash matching SSO names with the group names used to identify them
	//! in the configuration file.

	//! The names and group names of all objects in the default ssystem_major.ini.
	//! The keys are the names, the values are the group names.	
	QHash<QString, QString> defaultSsoIdentifiers;

	//! Extensive hash of comet cross-reference data.
	//! When importing MPC data, some extra info can be added from this.
	QHash<QString, CometDiscoveryData> cometCrossref;

	//! The list of discovery circumstances for numbered minor planets
	QHash<int, DiscoveryCircumstances> numberedMinorPlanets;

	//! The list of periodic comet number for comet-like asteroids
	QHash<int, QString> cometLikeAsteroids;

	//! Gets the names of the minor planet objects listed in a ssystem.ini-formatted file.
	//! Used internally in readAllCurrentSsoNames() and in init() to initialize
	//! defaultSsoNames.
	QHash<QString,QString> listAllLoadedObjectsInFile(const QString &filePath) const;

	//! Creates a copy of the default ssystem.ini file in the user data directory.
	//! \returns true if a file already exists or the copying has been successful
	bool cloneSolarSystemConfigurationFile() const;

	//! Replaces the user copy of ssystem.ini with the default one.
	//! This function simply deletes the file, if it exists, and calls
	//! cloneSolarSystemConfigurationFile().
	//! \returns true if the replacement has been successful.
	bool resetSolarSystemConfigurationFile() const;

	//! Check encoding of the file
	bool isFileEncodingValid(const QString &filePath) const;

	//! Load data for comets: designations and discovery circumstances
	void initCometCrossref();

	//! Load the list of discovery circumstances for numbered minor planets
	void initMinorPlanetData();

	//! Converts an alphanumeric digit as used in MPC packed dates to an integer.
	//! See http://www.minorplanetcenter.org/iau/info/PackedDates.html
	//! Interprets the digits from 0 to 9 normally, and the capital letters
	//! from A to V as numbers between 10 and 31.
	//! \returns -1 if the digit is invalid (0 is also an invalid ordinal number
	//! for a day or month, so this is not a problem)
	static int unpackDayOrMonthNumber (QChar digit);

	//! Converts an alphanumeric year number as used in MPC packed dates to an integer.
	//! See http://www.minorplanetcenter.org/iau/info/PackedDates.html
	//! Also used in packed IAU designations, see
	//! http://www.minorplanetcenter.org/iau/info/PackedDes.html
	static int unpackYearNumber (QChar prefix, int lastTwoDigits);



	//! Updates a value in a configuration file with a value with the same key in a SsoElements hash.
	static void updateSsoProperty(QSettings& configuration, const SsoElements& properties, const QString &key);

	//! Converts an object name to a key (group) name in a configuration file.
	static QString convertToGroupName(const QString &name, int minorPlanetNumber = 0);
	
	//! replaces "%25" by "%", then replaces "%28" by "(" and "%29" by ")".
	static QString fixGroupName(const QString &name);
};



#include <QObject>
#include "StelPluginInterface.hpp"

//! This class is used by Qt to manage a plug-in interface
class SolarSystemEditorStelPluginInterface : public QObject, public StelPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID StelPluginInterface_iid)
	Q_INTERFACES(StelPluginInterface)
public:
	StelModule* getStelModule() const override;
	StelPluginInfo getPluginInfo() const override;
	//QObjectList getExtensionList() const override { return QObjectList(); }
};

#endif // SOLARSYSTEMEDITOR_HPP
