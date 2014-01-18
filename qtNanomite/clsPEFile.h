/*
 * 	This file is part of Nanomite.
 *
 *    Nanomite is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Nanomite is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Nanomite.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CLSPEFILE_H
#define CLSPEFILE_H

#include <Windows.h>
#include <QStringlist.h>

struct APIData
{
	QString APIName;
	quint64 APIOffset;
};

struct SResourceDirectoryEntry;
struct SResourceDirectory {
	QList<SResourceDirectoryEntry> m_directoryEntries;
	IMAGE_RESOURCE_DIRECTORY m_dir;
};

struct SResourceDirectoryEntry {	
	SResourceDirectory m_sDir;
	IMAGE_RESOURCE_DATA_ENTRY m_dataEntry;

	IMAGE_RESOURCE_DIRECTORY_ENTRY m_directoryEntry;
	
	int m_level;
};

class clsPEFile
{
public:
	clsPEFile(QString FileName,bool *bLoaded, int PID = -1, quint64 imageBase = 0);
	~clsPEFile();

	bool isValidPEFile();
	
	QList<DWORD64> getTLSCallbackOffset();
	DWORD64 VAtoRaw(quint64 Offset);

	QList<APIData> getImports();
	QList<APIData> getExports();
	
	QList<IMAGE_SECTION_HEADER> getSections();

	IMAGE_DOS_HEADER getDosHeader();
	IMAGE_NT_HEADERS32 getNTHeader32();
	IMAGE_NT_HEADERS64 getNTHeader64();

	SResourceDirectory* getResourceDirectory();

	bool is64Bit();

	float getEntropie();

private:
	QString m_fileName;

	LPVOID m_fileBuffer;

	bool	m_is64Bit,
			m_isLoaded;

	QList<DWORD64> m_tlsOffset;

	IMAGE_DOS_HEADER m_IDH;
	IMAGE_FILE_HEADER m_IFH;
	IMAGE_NT_HEADERS64 m_INH64;
	IMAGE_NT_HEADERS32 m_INH32;

	QList<APIData> fileImports;
	QList<APIData> fileExports;

	QList<IMAGE_SECTION_HEADER> fileSections;

	float fileEntropie;

	QList<APIData> loadImports32();
	QList<APIData> loadImports64();
	QList<APIData> loadExports();
	void loadResource();
	
	QList<IMAGE_SECTION_HEADER> loadSections();

	bool LoadFile(QString fileName, int PID = -1, quint64 imageBase = 0);
	bool CheckFile(QString fileName);
	
	QList<DWORD64> loadTLSCallbackOffset64();
	QList<DWORD64> loadTLSCallbackOffset32();

	float loadEntropie(DWORD fileSize);

	SResourceDirectory m_rscrDir;
};

#endif