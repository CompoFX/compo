package com.ykode.compo;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.provider.MediaStore;
import android.net.Uri;
import android.database.Cursor;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;

import android.widget.Spinner;
import android.widget.Button;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.Toast;

import android.content.Intent;

import com.ykode.compo.CompoBridge;

import java.io.File;
import java.io.FileOutputStream;

import java.util.Date;
import java.text.SimpleDateFormat;

public class CompoDemoActivity extends Activity
{
  private static final int BROWSE_IMAGE_REQUEST = 1;

  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main);  

    final Spinner effectSpinner = (Spinner) this.findViewById(R.id.effectSpinner);
    final CompoView cv = (CompoView) this.findViewById(R.id.effectView);

    effectSpinner.setOnItemSelectedListener( new OnItemSelectedListener() {
      public void onItemSelected(AdapterView<?> parent, View view, 
        int pos, long id) {

        final CompoView.Effect [] effects = {
          CompoView.Effect.NORMAL,
      CompoView.Effect.GREYSCALE,
      CompoView.Effect.LIGHTEN,
      CompoView.Effect.SEPIA,
      CompoView.Effect.HITAMPUTIH,
      CompoView.Effect.SCRIBE,
      CompoView.Effect.HIKE,
      CompoView.Effect.CINTA,
      CompoView.Effect.ANALOG,
      CompoView.Effect.THERMO,
      CompoView.Effect.DESAKU,
      CompoView.Effect.KELUD,
      CompoView.Effect.HERO,
      CompoView.Effect.PENCIL,
      CompoView.Effect.STYLISH,
      CompoView.Effect.LORDKEVIN,
      CompoView.Effect.HUDSON,
      CompoView.Effect.PIXELATE 
        };
        cv.setEffect(effects[pos]); 
      }

      public void onNothingSelected(AdapterView<?> parent) {
        // Another interface callback
      }

    });

    Button browseButton = (Button) this.findViewById(R.id.selectImageButton);
    browseButton.setOnClickListener( new View.OnClickListener() {

      @Override
      public void onClick(View arg0) {
        Intent i = new Intent( Intent.ACTION_PICK,
          MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        startActivityForResult(i, BROWSE_IMAGE_REQUEST);
      }
    });

    final Button saveButton = (Button) this.findViewById(R.id.saveImageButton);
    saveButton.setOnClickListener( new View.OnClickListener() {
      
      @Override
      public void onClick(View v) {
        Bitmap bmp = cv.getResultImage();
        Log.v("[COMPOJAVASIDE]", "Got Image " + bmp.getWidth() + "x" + 
          bmp.getHeight());
        // getting filename
        Date date = new Date();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
        FileOutputStream fos = null;
        String basefn =  dateFormat.format(date) + ".jpg";
        String fn = Environment.getExternalStoragePublicDirectory(
          Environment.DIRECTORY_PICTURES) + "/" + basefn;

        try { 
          fos = new FileOutputStream(fn);
          bmp.compress(Bitmap.CompressFormat.JPEG, 80, fos);
        }
        catch(Exception e) {
          e.printStackTrace();
        }  
        finally {
          try {
            fos.close();
          }
          catch (Throwable ignore) {}
        }
      
        Intent mediaScanIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
        File f = new File(fn);
        Uri contentUri = Uri.fromFile(f);
        mediaScanIntent.setData(contentUri);
        saveButton.getContext().sendBroadcast(mediaScanIntent);
        
        Toast saveSucceeded = Toast.makeText(saveButton.getContext(),
            basefn + " saved successfully!", Toast.LENGTH_SHORT);
        saveSucceeded.show();
      }
    });
  }

  @Override
  protected void onActivityResult( int requestCode,
      int resultCode, Intent data)
  {
    super.onActivityResult(requestCode, resultCode, data);

    if (requestCode == BROWSE_IMAGE_REQUEST &&
        resultCode == RESULT_OK &&
        null != data)
    {
      final Uri selectedImage = data.getData();
      final String[] filePathColumn = {MediaStore.Images.Media.DATA};
      final Cursor cursor = getContentResolver().query(selectedImage,
          filePathColumn, null, null, null);
      cursor.moveToFirst();

      final int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
      final String picturePath = cursor.getString(columnIndex);
      cursor.close();

      Log.v("[COMPOJAVASIDE]", "Opened file '" + picturePath + "'");
    
      final CompoView v = (CompoView) this.findViewById(R.id.effectView);
      final Bitmap bmp = BitmapFactory.decodeFile(picturePath);
      v.setBitmap(bmp);
    } 
  }

  protected void finalize() throws Throwable {
    Log.v("[COMPOJAVASIDE]", "Activity Object Finalized!");
    super.finalize();
  }
}
