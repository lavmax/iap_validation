iap_validation
================

In July 2012, a Russian hacker identified a vulnerability in the In-App Purchase system and released it into the wild. Apple responded with TP40012484, "In-App Purchase Receipt Validation on iOS," which included VerificationController, a mostly-complete singleton class to perform validation.

Apple's VerificationController requires making a number of changes within the code and  adding your own callbacks and base64 implementation. This small collection of classes is based on it and provides a simple delegate-based system to perform validation. 

It's in the public domain with USE_CODE_REQUIRING_ATTRIBUTION not defined in RRBase64Manager. If it *is* defined, which is the default, a more efficient base64 implementation which does require attribution will be used; see that file for details.

Use
--------

This example assumes that your `SKPaymentTransactionObserver` conformant class will also implement `RRVerificationControllerDelegate`.

You'll need your iTunes Connect In App Purchase Shared Secret, which you can find/generate via **iTunes Connect -> Manage Apps -> your app -> Manage In App Purchases**.

```
 #define MY_SHARED_SECRET	@"1234567890abcdef1234567890abcdef"
 
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
```

RRVerificationControllerDelegate is implemented like so:

```
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
```
Everything else is handled automatically; just put your own logic in performUpgrade, failedTransaction:, and displayFailureMessage, with no changes needed to the verification controller code.

These classes are intended to be compiled with ARC enabled. Be sure to specify `-fobjc-arc` in the 'Compile Sources' Build Phase for each file if you aren't using ARC project-wide 