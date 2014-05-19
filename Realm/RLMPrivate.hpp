////////////////////////////////////////////////////////////////////////////
//
// TIGHTDB CONFIDENTIAL
// __________________
//
//  [2011] - [2014] TightDB Inc
//  All Rights Reserved.
//
// NOTICE:  All information contained herein is, and remains
// the property of TightDB Incorporated and its suppliers,
// if any.  The intellectual and technical concepts contained
// herein are proprietary to TightDB Incorporated
// and its suppliers and may be covered by U.S. and Foreign Patents,
// patents in process, and are protected by trade secret or copyright law.
// Dissemination of this information or reproduction of this material
// is strictly forbidden unless prior written permission is obtained
// from TightDB Incorporated.
//
////////////////////////////////////////////////////////////////////////////

#import <Foundation/Foundation.h>
#import "RLMRealm.h"
#import "RLMArray.h"
#import "RLMAccesor.h"

#import <tightdb/table.hpp>
#import <tightdb/group.hpp>
#import <tightdb/table_view.hpp>

// RLMRealm transaction state
typedef NS_ENUM(NSUInteger, RLMTransactionMode) {
    RLMTransactionModeNone = 0,
    RLMTransactionModeRead,
    RLMTransactionModeWrite
};

// RLMRealm members used by the object store
@interface RLMRealm ()
@property (nonatomic, readonly) RLMTransactionMode transactionMode;
@property (nonatomic, readonly) tightdb::Group *group;

// call whenever creating an accessor to keep up to date accross transactions
- (void)registerAccessor:(id<RLMAccessor>)accessor;
@end

// RLMObject accessor and read/write realm
@interface RLMObject () <RLMAccessor>
@property (nonatomic, readwrite) RLMRealm *realm;
@end

// RLMArray private members and accessor
@interface RLMArray () <RLMAccessor>
@property (nonatomic, assign) tightdb::Query *backingQuery;
@property (nonatomic, assign) tightdb::TableView backingView;
@property (nonatomic, assign) Class objectClass;
@end


