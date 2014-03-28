/*************************************************************************
 *
 * TIGHTDB CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2014] TightDB Inc
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of TightDB Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to TightDB Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from TightDB Incorporated.
 *
 **************************************************************************/

#ifndef TIGHTDB_OBJC_SUPPORT_H
#define TIGHTDB_OBJC_SUPPORT_H

#import <Foundation/Foundation.h>
#include <tightdb/descriptor.hpp>

void to_mixed(id value, tightdb::Mixed& m);

BOOL set_cell(size_t col_ndx, size_t row_ndx, tightdb::Table& table, NSObject *obj);
BOOL verify_cell(const tightdb::Descriptor& descr, size_t col_ndx, NSObject *obj);

void verify_row(const tightdb::Descriptor& descr, NSArray * data);
void insert_row(size_t ndx, tightdb::Table& table, NSArray * data);
void set_row(size_t ndx, tightdb::Table& table, NSArray *data);

void verify_row_with_labels(const tightdb::Descriptor& descr, NSDictionary* data);
void insert_row_with_labels(size_t row_ndx, tightdb::Table& table, NSDictionary *data);
void set_row_with_labels(size_t row_ndx, tightdb::Table& table, NSDictionary *data);
#endif /* TIGHTDB_OBJC_SUPPORT_H */