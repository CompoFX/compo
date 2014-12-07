//
//  ViewController.m
//  CompoViewDemo
//
//  Created by Didiet Noor on 18/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#import "ViewController.h"
#import "compoView.h"

@interface ViewController () {
  NSArray *effectArray;
}
@property (weak, nonatomic) IBOutlet UIView *compoView;
@property (weak, nonatomic) IBOutlet UIPickerView *effectPicker;
- (IBAction)saveButtonPressed:(id)sender;
- (IBAction)pickButtonPressed:(id)sender;
- (void) image: (UIImage*) image
         didFinishSavingWithError: (NSError*) error
         contextInfo: (void*) context;
@end

@implementation ViewController
@synthesize compoView = mCompoView;
@synthesize effectPicker = mEffectPicker;

- (void)viewDidLoad
{
  [super viewDidLoad];
  
  effectArray = [[NSArray alloc] initWithObjects:@"Normal", @"Grey Scale", @"Lighten", @"Sepia", @"Hitam Putih", @"Scribe", @"Hike", @"Cinta", @"Analog", @"Thermo", @"Desaku", @"Kelud", @"Hero", @"Pencil",
    @"Stylish", @"Lord Kelvin", @"Hudson", @"Pixelate", nil];
  
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (NSInteger) numberOfComponentsInPickerView:(UIPickerView *) picker
{
  return 1;
}

- (NSInteger) pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
  return 18;
}

- (NSString*) pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
  return [effectArray objectAtIndex:row];
}

- (void) pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
  [(CompoView*)self.compoView setEffect:(CompoViewEffectType) row];
}


- (IBAction)pickButtonPressed:(id)sender {
  UIImagePickerController *ipc = [[UIImagePickerController alloc] init];
  ipc.delegate = self;
  ipc.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
  ipc.allowsEditing = NO;
  ipc.navigationBar.tintColor = [UIColor blueColor];
  ipc.navigationBar.backgroundColor = [UIColor darkGrayColor];
  ipc.modalPresentationStyle = UIModalPresentationFormSheet;
  [self presentViewController:ipc animated:YES completion:nil];
}

- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
  UIImage *originalImage = [info valueForKey:
                            UIImagePickerControllerOriginalImage];
  CompoView *cView = (CompoView*) mCompoView;
  cView.image = originalImage;
  [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)saveButtonPressed:(id)sender
{
  CompoView *cv = (CompoView*) mCompoView;
  UIImage *result = cv.resultImage;
  
  dispatch_async(dispatch_get_main_queue(), ^{
    UIImageWriteToSavedPhotosAlbum(result, self,
        @selector(image:didFinishSavingWithError:contextInfo:), NULL);
  });
}

- (void) image: (UIImage*) image didFinishSavingWithError: (NSError*) error
   contextInfo: (void*) context
{
  UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Success" message:@"Saved"
                                                 delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles: nil];
  [alert show];
}
                
                                 
@end
