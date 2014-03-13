/*************************************************************************
 *
 * TIGHTDB CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2012] TightDB Inc
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

#import <Foundation/Foundation.h>

#include <tightdb/objc/type.h>

@class TightdbTable;
@class TightdbView;
@class TightdbQuery;
@class TightdbCursor;


@interface TightdbBinary: NSObject
-(id)initWithData:(const char *)data size:(size_t)size;
-(const char *)getData;
-(size_t)getSize;

/**
 * Compare the referenced binary data for equality.
 */
-(BOOL)isEqual:(TightdbBinary *)bin;
@end


@interface TightdbMixed: NSObject
+(TightdbMixed *)mixedWithBool:(BOOL)value;
+(TightdbMixed *)mixedWithInt64:(int64_t)value;
+(TightdbMixed *)mixedWithFloat:(float)value;
+(TightdbMixed *)mixedWithDouble:(double)value;
+(TightdbMixed *)mixedWithString:(NSString *)value;
+(TightdbMixed *)mixedWithBinary:(TightdbBinary *)value;
+(TightdbMixed *)mixedWithBinary:(const char *)data size:(size_t)size;
+(TightdbMixed *)mixedWithDate:(time_t)value;
+(TightdbMixed *)mixedWithTable:(TightdbTable *)value;
-(BOOL)isEqual:(TightdbMixed *)other;
-(TightdbType)getType;
-(BOOL)getBool;
-(int64_t)getInt;
-(float)getFloat;
-(double)getDouble;
-(NSString *)getString;
-(TightdbBinary *)getBinary;
-(time_t)getDate;
-(TightdbTable *)getTable;
@end


@interface TightdbDescriptor: NSObject

@property (nonatomic, readonly) NSUInteger columnCount;

/**
 * Returns NO on memory allocation error.
 */
-(BOOL)addColumnWithName:(NSString *)name andType:(TightdbType)type;
-(BOOL)addColumnWithName:(NSString *)name andType:(TightdbType)type error:(NSError *__autoreleasing *)error;
/**
 * Returns nil on memory allocation error.
 */
-(TightdbDescriptor *)addColumnTable:(NSString *)name;
-(TightdbDescriptor *)addColumnTable:(NSString *)name error:(NSError *__autoreleasing *)error;
-(TightdbDescriptor *)subdescriptorForColumnWithIndex:(NSUInteger)colIndex;
-(TightdbDescriptor *)subdescriptorForColumnWithIndex:(NSUInteger)colIndex error:(NSError *__autoreleasing *)error;

-(TightdbType)columnTypeOfColumn:(NSUInteger)colIndex;
-(NSString *)columnNameOfColumn:(NSUInteger)colIndex;
-(NSUInteger)indexOfColumnWithName:(NSString *)name;
@end


@interface TightdbTable: NSObject <NSFastEnumeration>

@property (nonatomic, readonly) NSUInteger columnCount;
@property (nonatomic, readonly) TightdbDescriptor *descriptor;
@property (nonatomic, readonly) NSUInteger rowCount;

-(NSUInteger)countByEnumeratingWithState:(NSFastEnumerationState *)state objects:(id __unsafe_unretained *)stackbuf count:(NSUInteger)len;

-(BOOL)isEqual:(TightdbTable *)other;

-(BOOL)isReadOnly;


/**
 * This method will return NO if it encounters a memory allocation
 * error (out of memory).
 *
 * The specified table class must be one that is declared by using
 * one of the table macros TIGHTDB_TABLE_*.
 */
-(BOOL)hasSameDescriptorAs:(Class)otherTableClass;

/**
 * If the type of this table is not compatible with the specified
 * table class, then this method returns nil. It also returns nil if
 * it encounters a memory allocation error (out of memory).
 *
 * The specified table class must be one that is declared by using
 * one of the table macros TIGHTDB_TABLE_*.
 */
-(id)castClass:(Class)obj;

/* Column meta info */
-(NSUInteger)addColumnWithName:(NSString *)name andType:(TightdbType)type;
-(void)removeColumnWithIndex:(NSUInteger)colIndex;
-(NSString *)columnNameOfColumn:(NSUInteger)colIndex;
-(NSUInteger)indexOfColumnWithName:(NSString *)name;
-(TightdbType)columnTypeOfColumn:(NSUInteger)colIndex;


-(TightdbCursor *)addEmptyRow;

/* Only curser based add should be public. This is just a temporaray way to hide the methods. */
/* TODO: Move to class extension. */
-(NSUInteger)TDBAddEmptyRow;
-(NSUInteger)TDBAddEmptyRows:(NSUInteger)numberOfRows;

-(BOOL)removeAllRows;
-(BOOL)removeRowAtIndex:(NSUInteger)rowIndex;
-(BOOL)removeLastRow;

-(TightdbCursor *)objectAtIndexedSubscript:(NSUInteger)ndx; /* object subscripting */
-(TightdbCursor *)rowAtIndex:(NSUInteger)rowIndex;
-(TightdbCursor *)lastRow;
-(TightdbCursor *)firstRow;

-(TightdbCursor *)insertEmptyRowAtIndex:(NSUInteger)ndx;

-(BOOL)appendRow:(NSArray *)data;

-(BOOL)boolInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(int64_t)intInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(float)floatInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(double)doubleInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(time_t)dateInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(NSString *)stringInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(TightdbBinary *)binaryInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(TightdbTable *)tableInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(id)tableInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex asTableClass:(Class)tableClass;
-(TightdbMixed *)mixedInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;

-(void)setInt:(int64_t)anInt inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(void)setBool:(BOOL)aBool inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)atRowIndex;
-(void)setFloat:(float)aFloat inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)atRowIndex;
-(void)setDouble:(double)aDouble inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)atRowIndex;
-(void)setDate:(time_t)aDate inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)atRowIndex;
-(void)setString:(NSString *)aString inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)atRowIndex;
-(void)setBinary:(TightdbBinary *)aBinary inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)atRowIndex;
-(void)setTable:(TightdbTable *)aTable inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)atRowIndex;
-(void)setMixed:(TightdbMixed *)aMixed inColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)atRowIndex;



-(BOOL)TDBInsertBool:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(BOOL)value;
-(BOOL)TDBInsertInt:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(int64_t)value;
-(BOOL)TDBInsertFloat:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(float)value;
-(BOOL)TDBInsertDouble:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(double)value;
-(BOOL)TDBInsertString:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(NSString *)value;
-(BOOL)TDBInsertBinary:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(TightdbBinary *)value;
-(BOOL)TDBInsertBinary:(NSUInteger)colIndex ndx:(NSUInteger)ndx data:(const char *)data size:(size_t)size;
-(BOOL)TDBInsertDate:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(time_t)value;
-(BOOL)TDBInsertDone;


/* Subtables */

-(BOOL)TDBInsertSubtable:(NSUInteger)colIndex ndx:(NSUInteger)ndx;
-(BOOL)TDBInsertSubtable:(NSUInteger)colIndex ndx:(NSUInteger)ndx error:(NSError *__autoreleasing *)error;


/* Mixed */

-(TightdbType)mixedTypeForColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(BOOL)TDBInsertMixed:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(TightdbMixed *)value;
-(BOOL)TDBInsertMixed:(NSUInteger)colIndex ndx:(NSUInteger)ndx value:(TightdbMixed *)value error:(NSError *__autoreleasing *)error;




-(NSUInteger)findRowIndexWithBool:(BOOL)aBool inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)findRowIndexWithInt:(int64_t)anInt inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)findRowIndexWithFloat:(float)aFloat inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)findRowIndexWithDouble:(double)aDouble inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)findRowIndexWithString:(NSString *)aString inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)findRowIndexWithBinary:(TightdbBinary *)aBinary inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)findRowIndexWithDate:(time_t)aDate inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)findRowIndexWithMixed:(TightdbMixed *)aMixed inColumnWithIndex:(NSUInteger)colIndex;


-(TightdbView *)findAllRowsWithBool:(BOOL)aBool inColumnWithIndex:(NSUInteger)colIndex;
-(TightdbView *)findAllRowsWithInt:(int64_t)anInt inColumnWithIndex:(NSUInteger)colIndex;
-(TightdbView *)findAllRowsWithFloat:(float)aFloat inColumnWithIndex:(NSUInteger)colIndex;
-(TightdbView *)findAllRowsWithDouble:(double)aDouble inColumnWithIndex:(NSUInteger)colIndex;
-(TightdbView *)findAllRowsWithString:(NSString *)aString inColumnWithIndex:(NSUInteger)colIndex;
-(TightdbView *)findAllRowsWithBinary:(TightdbBinary *)aBinary inColumnWithIndex:(NSUInteger)colIndex;
-(TightdbView *)findAllRowsWithDate:(time_t)aDate inColumnWithIndex:(NSUInteger)colIndex;
-(TightdbView *)findAllRowsWithMixed:(TightdbMixed *)aMixed inColumnWithIndex:(NSUInteger)colIndex;

-(TightdbQuery *)where;
-(TightdbQuery *)whereWithError:(NSError *__autoreleasing *)error;

/* Indexing */
-(void)createIndexInColumnWithIndex:(NSUInteger)colIndex;
-(BOOL)isIndexCreatedInColumnWithIndex:(NSUInteger)colIndex;

/* Optimizing */
-(BOOL)optimize;
-(BOOL)optimizeWithError:(NSError *__autoreleasing *)error;

/* Conversion */
/* FIXME: Do we want to conversion methods? Maybe use NSData. */

/* Aggregate functions */
/* FIXME: Consider adding:
 countRowsWithValue: @"foo"
 countRowsWithValue: @300 */
-(NSUInteger)countRowsWithInt:(int64_t)anInt inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)countRowsWithFloat:(float)aFloat inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)countRowsWithDouble:(double)aDouble inColumnWithIndex:(NSUInteger)colIndex;
-(NSUInteger)countRowsWithString:(NSString *)aString inColumnWithIndex:(NSUInteger)colIndex;
-(int64_t)sumIntColumnWithIndex:(NSUInteger)colIndex;
-(double)sumFloatColumnWithIndex:(NSUInteger)colIndex;
-(double)sumDoubleColumnWithIndex:(NSUInteger)colIndex;
-(int64_t)maxIntInColumnWithIndex:(NSUInteger)colIndex;
-(float)maxFloatInColumnWithIndex:(NSUInteger)colIndex;
-(double)maxDoubleInColumnWithIndex:(NSUInteger)colIndex;
-(int64_t)minIntInColumnWithIndex:(NSUInteger)colIndex;
-(float)minFloatInColumnWithIndex:(NSUInteger)colIndex;
-(double)minDoubleInColumnWithIndex:(NSUInteger)colIndex;
-(double)avgIntColumnWithIndex:(NSUInteger)colIndex;
-(double)avgFloatColumnWithIndex:(NSUInteger)colIndex;
-(double)avgDoubleColumnWithIndex:(NSUInteger)colIndex;

#ifdef TIGHTDB_DEBUG
-(void)verify;
#endif

/* Private */
-(id)_initRaw;
-(BOOL)TDBInsertSubtableCopy:(NSUInteger)colIndex row:(NSUInteger)rowNdx subtable:(TightdbTable *)subtable;
-(BOOL)TDBInsertSubtableCopy:(NSUInteger)colIndex row:(NSUInteger)rowIndex subtable:(TightdbTable *)subtable error:(NSError *__autoreleasing *)error;
@end


@interface TightdbView: NSObject <NSFastEnumeration>

@property (nonatomic, readonly) NSUInteger rowCount;
@property (nonatomic, readonly) NSUInteger columnCount;
@property (nonatomic, readonly) TightdbTable *originTable;

-(TightdbCursor *)rowAtIndex:(NSUInteger)rowIndex;

-(TightdbType)columnTypeOfColumn:(NSUInteger)colIndex;

-(void) sortUsingColumnWithIndex: (NSUInteger)colIndex;
-(void) sortUsingColumnWithIndex: (NSUInteger)colIndex inOrder: (TightdbSortOrder)order;


-(BOOL)boolInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(int64_t)intInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(float)floatInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(double)doubleInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(time_t)dateInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
-(NSString *)stringInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
//-(TightdbBinary *)binaryInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
//-(TightdbTable *)tableInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;
//-(id)tableInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex asTableClass:(Class)tableClass;
-(TightdbMixed *)mixedInColumnWithIndex:(NSUInteger)colIndex atRowIndex:(NSUInteger)rowIndex;


-(void)removeRowAtIndex:(NSUInteger)atRowIndex;
-(void)removeAllRows;

-(NSUInteger)rowIndexInOriginTableForRowAtIndex:(NSUInteger)rowIndex;

- (NSUInteger)countByEnumeratingWithState:(NSFastEnumerationState *)state objects:(id __unsafe_unretained *)stackbuf count:(NSUInteger)len;

/* Private */
-(id)_initWithQuery:(TightdbQuery *)query;
@end


@interface TightdbColumnProxy: NSObject
@property(nonatomic, weak) TightdbTable *table;
@property(nonatomic) size_t column;
-(id)initWithTable:(TightdbTable *)table column:(NSUInteger)column;
-(void)clear;
@end

@interface TightdbColumnProxy_Bool: TightdbColumnProxy
-(NSUInteger)find:(BOOL)value;
@end

@interface TightdbColumnProxy_Int: TightdbColumnProxy
-(NSUInteger)find:(int64_t)value;
-(int64_t)minimum;
-(int64_t)maximum;
-(int64_t)sum;
-(double)average;
@end

@interface TightdbColumnProxy_Float: TightdbColumnProxy
-(NSUInteger)find:(float)value;
-(float)minimum;
-(float)maximum;
-(double)sum;
-(double)average;
@end

@interface TightdbColumnProxy_Double: TightdbColumnProxy
-(NSUInteger)find:(double)value;
-(double)minimum;
-(double)maximum;
-(double)sum;
-(double)average;
@end

@interface TightdbColumnProxy_String: TightdbColumnProxy
-(NSUInteger)find:(NSString *)value;
@end

@interface TightdbColumnProxy_Binary: TightdbColumnProxy
-(NSUInteger)find:(TightdbBinary *)value;
@end

@interface TightdbColumnProxy_Date: TightdbColumnProxy
-(NSUInteger)find:(time_t)value;
@end

@interface TightdbColumnProxy_Subtable: TightdbColumnProxy
@end

@interface TightdbColumnProxy_Mixed: TightdbColumnProxy
-(NSUInteger)find:(TightdbMixed *)value;
@end