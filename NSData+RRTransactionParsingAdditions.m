//
//  NSData+RRTransactionParsingAdditions.m
//  Rowmote
//
//  Created by Evan Schoenberg on 7/20/12.
//
//

#import "NSData+RRTransactionParsingAdditions.h"

@implementation NSData (RRTransactionParsingAdditions)

- (NSDictionary *)rr_dictionaryFromPlistDataWithError:(out NSError **)outError
{
    NSDictionary *dictionaryParsed = [NSPropertyListSerialization propertyListWithData:self
                                                                               options:NSPropertyListImmutable
                                                                                format:nil
                                                                                 error:outError];
    return dictionaryParsed;
}


- (NSDictionary *)rr_dictionaryFromJSONDataWithError:(out NSError **)outError
{
    NSDictionary *dictionaryParsed = [NSJSONSerialization JSONObjectWithData:self
                                                                     options:0
                                                                       error:outError];
    return dictionaryParsed;
}

@end
