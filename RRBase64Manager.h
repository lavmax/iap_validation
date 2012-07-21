//
//  RRBase64Manager.h
//  Rowmote
//
//  Created by Evan Schoenberg on 7/20/12.
//
//

#import <Foundation/Foundation.h>

@interface RRBase64Manager : NSObject

+ (NSString *)encodeBase64WithBytes:(const unsigned char *)objRawData length:(size_t)intLength;
+ (NSString *)encodeBase64WithData:(NSData *)objData;

+ (NSData *)decodeBase64WithString:(NSString *)strBase64;

@end
