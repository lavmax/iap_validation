#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#define ITMS_PROD_VERIFY_RECEIPT_URL        @"https://buy.itunes.apple.com/verifyReceipt"
#define ITMS_SANDBOX_VERIFY_RECEIPT_URL     @"https://sandbox.itunes.apple.com/verifyReceipt";

#ifdef DEBUG_BUILD
	// Use ITMS_SANDBOX_VERIFY_RECEIPT_URL while testing against the sandbox.
	#define ITMS_ACTIVE_VERIFY_RECEIPT_URL	ITMS_SANDBOX_VERIFY_RECEIPT_URL
#else
	#define ITMS_ACTIVE_VERIFY_RECEIPT_URL	ITMS_PROD_VERIFY_RECEIPT_URL
#endif


@class RRTransactionVerifier;

@protocol RRVerificationControllerDelegate
- (void)verificationControllerDidVerifyPurchase:(SKPaymentTransaction *)transaction isValid:(BOOL)isValid;
- (void)verificationControllerDidFailToVerifyPurchase:(SKPaymentTransaction *)transaction error:(NSError *)error;
@end

@interface RRVerificationController : NSObject {
    NSMutableDictionary *transactionsReceiptStorageDictionary;
	
	NSMutableArray *verificationsInProgress;
}

+ (RRVerificationController *) sharedInstance;

/* Must be set before attempting to verify. Generated via iTunes Connect -> Manage Apps -> your app -> Manage In App Purchases */
@property (strong) NSString *itcContentProviderSharedSecret;

/*!
 * @brief Verify a purchase
 *
 * This is an asynchronous process that requires checking with Apple's server to determine validity.
 * verificationDelegate will be sent verificationControllerDidVerifyPurchase:isValid: when verficiation is complete, if it begins at all
 *
 * This method should be called once a transaction gets to the SKPaymentTransactionStatePurchased or SKPaymentTransactionStateRestored state.
 *
 * @result YES if verification begins; NO if verification failed immediately, in which case the delegate will not be called.
 */
- (BOOL)verifyPurchase:(SKPaymentTransaction *)transaction withDelegate:(NSObject<RRVerificationControllerDelegate> *)verificationDelegate  error:(out NSError **)outError;

@end

@interface RRVerificationController (ForTransactionVerifierOnly)
- (void)transactionVerifier:(RRTransactionVerifier *)verifier didDetermineValidity:(BOOL)isValid;
- (void)transactionVerifier:(RRTransactionVerifier *)verifier didFailWithError:(NSError *)error;
@end


/*
 The calling implementation looks something like:
 
 #define MY_SHARED_SECRET	@"1234567890 abcdef1234 567890abcd ef"
 
- (void)paymentQueue:(id)queue updatedTransactions:(NSArray *)transactions
{
	[RRVerificationController sharedInstance].itcContentProviderSharedSecret = MY_SHARED_SECRET;
 
	for (SKPaymentTransaction *transaction in transactions)
	{
		switch ([transaction transactionState])
		{
			case SKPaymentTransactionStatePurchasing:
				break;
			case SKPaymentTransactionStatePurchased:
				if ([[RRVerificationController sharedInstance] verifyPurchase:transaction
																 withDelegate:self
																		error:NULL] == FALSE) {
					[self failedTransaction:transaction];
				}
				break;
			case SKPaymentTransactionStateFailed:
				[self failedTransaction:transaction];
				break;
			case SKPaymentTransactionStateRestored:
				if ([[RRVerificationController sharedInstance] verifyPurchase:transaction
																 withDelegate:self
																		error:NULL] == FALSE) {
					[self failedTransaction:transaction];
				}
			default:
				break;
		}
	}
}
*/

/*
 The delegate implementation looks something like:

 - (void)verificationControllerDidVerifyPurchase:(SKPaymentTransaction *)transaction isValid:(BOOL)isValid
 {
	if (isValid)
		[self performUpgrade];
	else
		[self displayFailureMessage];
 }
 
 - (void)verificationControllerDidFailToVerifyPurchase:(SKPaymentTransaction *)transaction error:(NSError *)error
 {
	NSString *message = NSLocalizedString(@"Your purchase could not be verified with Apple's servers. Please try again later.", nil);
	if (error) {
		message = [message stringByAppendingString:@"\n\n"];
		message = [message stringByAppendingFormat:NSLocalizedString(@"The error was: %@.", nil), error.localizedDescription];
	}
 
	[[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Purchase Verification Failed", nil)
		message:message
		delegate:nil
		cancelButtonTitle:NSLocalizedString(@"Dismiss", nil)
		otherButtonTitles:nil] show];
 }
 */

