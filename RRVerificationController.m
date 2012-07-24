/* RRVerificationController.m
 *
 *  Created by Evan Schoenberg on 7/20/12.
 *  Copyright 2012 Regular Rate and Rhythm Software. No rights reserved.
 *
 * Completed and fleshed out implementation of Apple's VerificationController, the Companion File to
 * TP40012484 ("In-App Purchase Receipt Validation on iOS" -  bit.ly/QiosJw)
 *
 * Public domain. Feel free to give me credit or send brownies if you'd like.
 */

#if ! __has_feature(objc_arc)
#warning This file must be compiled with ARC. Use -fobjc-arc flag (or convert project to ARC).
#endif

#import "RRVerificationController.h"
#import "RRTransactionVerifier.h"

@implementation RRVerificationController

@synthesize itcContentProviderSharedSecret;

+ (RRVerificationController *)sharedInstance
{
	static RRVerificationController *singleton = nil;

	if (singleton == nil)
		singleton = [[RRVerificationController alloc] init];

	return singleton;
}


- (id)init
{
	if ((self = [super init])) {
		verificationsInProgress = [[NSMutableArray alloc] init];
	}

	return self;
}

#pragma mark Receipt Verification

// This method should be called once a transaction gets to the SKPaymentTransactionStatePurchased or SKPaymentTransactionStateRestored state
- (BOOL)verifyPurchase:(SKPaymentTransaction *)transaction withDelegate:(NSObject<RRVerificationControllerDelegate> *)verificationDelegate error:(out NSError **)outError;
{
	if ((transaction.transactionState == SKPaymentTransactionStatePurchased) || (transaction.transactionState == SKPaymentTransactionStateRestored)) {
		RRTransactionVerifier *verifier = [[RRTransactionVerifier alloc] initWithPurchase:transaction delegate:verificationDelegate controller:self];
		[verificationsInProgress addObject:verifier];
		
		return [verifier beginVerificationWithError:outError];
	} else {
		/* Transaction wasn't in a state in which it can possibly be valid */
		if (outError)
			*outError = [NSError errorWithDomain:SKErrorDomain code:SKErrorPaymentInvalid userInfo:nil];
		return NO;
	}
}

- (void)transactionVerifier:(RRTransactionVerifier *)verifier didDetermineValidity:(BOOL)isValid
{
	[verifier.delegate verificationControllerDidVerifyPurchase:verifier.transaction isValid:isValid];
	[verificationsInProgress removeObject:verifier];
}


- (void)transactionVerifier:(RRTransactionVerifier *)verifier didFailWithError:(NSError *)error
{
	[verifier.delegate verificationControllerDidFailToVerifyPurchase:verifier.transaction error:error];
	[verificationsInProgress removeObject:verifier];
}


@end
