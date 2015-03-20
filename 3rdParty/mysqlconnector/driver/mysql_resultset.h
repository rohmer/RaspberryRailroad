/*
Copyright (c) 2008, 2014, Oracle and/or its affiliates. All rights reserved.

The MySQL Connector/C++ is licensed under the terms of the GPLv2
<http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>, like most
MySQL Connectors. There are special exceptions to the terms and
conditions of the GPLv2 as it is applied to this software, see the
FLOSS License Exception
<http://www.mysql.com/about/legal/licensing/foss-exception.html>.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/



#ifndef _MYSQL_RESULTSET_H_
#define _MYSQL_RESULTSET_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <cppconn/resultset.h>

#include "nativeapi/mysql_private_iface.h"

namespace sql
{
namespace mysql
{
class MySQL_Statement;
class MySQL_DebugLogger;
class MySQL_ResultSetMetaData;

namespace NativeAPI
{
	class NativeResultsetWrapper;
}

class MySQL_ResultSet : public sql::ResultSet
{
	MYSQL_ROW				row;
	boost::shared_ptr< NativeAPI::NativeResultsetWrapper > result;
	boost::weak_ptr< NativeAPI::NativeConnectionWrapper > proxy;
	unsigned int			num_fields;
	uint64_t				num_rows;
	uint64_t				row_position;
	/* 0 = before first row, 1 - first row, 'num_rows + 1' - after last row */

	typedef std::map< sql::SQLString, unsigned int > FieldNameIndexMap;

	FieldNameIndexMap	field_name_to_index_map;
	mutable bool		was_null;

	const MySQL_Statement * parent;

	boost::shared_ptr< MySQL_DebugLogger > logger;

	std::auto_ptr< MySQL_ResultSetMetaData > rs_meta;

	sql::ResultSet::enum_type resultset_type;

protected:
	void checkValid() const;
	void checkScrollable() const;
	bool isScrollable() const;
	bool isBeforeFirstOrAfterLast() const;
	void seek();

	MYSQL_FIELD * getFieldMeta(unsigned int columnIndex) const;

public:
	MySQL_ResultSet(boost::shared_ptr< NativeAPI::NativeResultsetWrapper > res, boost::weak_ptr< NativeAPI::NativeConnectionWrapper > _proxy, sql::ResultSet::enum_type rset_type, MySQL_Statement * par,
					boost::shared_ptr< MySQL_DebugLogger > & l);

	virtual ~MySQL_ResultSet();

	bool absolute(int row);

	void afterLast();

	void beforeFirst();

	void cancelRowUpdates();

	void clearWarnings();

	void close();

	uint32_t findColumn(const sql::SQLString& columnLabel) const;

	bool first();

	std::istream * getBlob(uint32_t columnIndex) const;
	std::istream * getBlob(const sql::SQLString& columnLabel) const;

	bool getBoolean(uint32_t columnIndex) const;
	bool getBoolean(const sql::SQLString& columnLabel) const;

	int getConcurrency();

	SQLString getCursorName();

	long double getDouble(uint32_t columnIndex) const;
	long double getDouble(const sql::SQLString& columnLabel) const;

	int getFetchDirection();

	size_t getFetchSize();

	int getHoldability();

	int32_t getInt(uint32_t columnIndex) const;
	int32_t getInt(const sql::SQLString& columnLabel) const;

	uint32_t getUInt(uint32_t columnIndex) const;
	uint32_t getUInt(const sql::SQLString& columnLabel) const;

	int64_t getInt64(uint32_t columnIndex) const;
	int64_t getInt64(const sql::SQLString& columnLabel) const;

	uint64_t getUInt64(uint32_t columnIndex) const;
	uint64_t getUInt64(const sql::SQLString& columnLabel) const;

	sql::ResultSetMetaData * getMetaData() const;

	size_t getRow() const;

	sql::RowID * getRowId(uint32_t columnIndex);
	sql::RowID * getRowId(const sql::SQLString & columnLabel);

	const sql::Statement * getStatement() const;

	SQLString getString(uint32_t columnIndex) const;
	SQLString getString(const sql::SQLString& columnLabel) const;

	sql::ResultSet::enum_type getType() const;

	void getWarnings();

	void insertRow();

	bool isAfterLast() const;

	bool isBeforeFirst()const;

	bool isClosed() const;

	bool isFirst() const;

	bool isLast() const;

	bool isNull(uint32_t columnIndex) const;

	bool isNull(const sql::SQLString& columnLabel) const;

	bool last();

	void moveToCurrentRow();

	void moveToInsertRow();

	bool next();

	bool previous();

	void refreshRow();

	bool relative(int rows);

	bool rowDeleted();

	bool rowInserted();

	bool rowUpdated();

	size_t rowsCount() const;

	void setFetchSize(size_t rows);

	bool wasNull() const;
private:
	/* Prevent use of these */
	MySQL_ResultSet(const MySQL_ResultSet &);
	void operator=(MySQL_ResultSet &);
};

} /* namespace mysql */
} /* namespace sql */
#endif // _MYSQL_RESULTSET_H_

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
