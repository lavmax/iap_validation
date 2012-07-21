//
//  NSData+RRTransactionParsingAdditions.h
//  Rowmote
//
//  Created by Evan Schoenberg on 7/20/12.
//
//

#import <Foundation/Foundation.h>

@interface NSData (RRTransactionParsingAdditions)

- (NSDictionary *)rr_dictionaryFromPlistDataWithError:(out NSError **)outError;
- (NSDictionary *)rr_dictionaryFromJSONDataWithError:(out NSError **)outError;

@end
