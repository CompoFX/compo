/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.2
import bb.cascades.pickers 1.0
import bb.system 1.0

import com.compofx 1.0

Page {
    Container {
        
        Label {
            // Localized text with the dynamic translation and locale updates support
            text: qsTr("Compo Demo") + Retranslate.onLocaleOrLanguageChanged
            textStyle.base: SystemDefaults.TextStyles.BigText
            id: outLabel
            leftMargin: 0.0

        }
        Container {
            layout: DockLayout {
                
            }
            leftPadding: 10.0
            rightPadding: 10.0
            bottomPadding: 10.0
            topPadding: 10.0
            CompoView
            {
                id: compo;
                preferredWidth: 700;
                preferredHeight: 700;
            } 
        }
        DropDown {
            id: ddEffect
            title: qsTr("Effects")
            options: [
            	Option {
                	text: "Normal"
                	description: qsTr("Normal Effect")
                },
            	Option {
                	text: "Greyscale"
                	description: qsTr("GrayScale Effect, color averaging") 
                },
                Option {
                    text: "Lighten"
                     description: qsTr("Bleaching the color") 
                },
                Option {
                    text: "Sepia"
                    description: qsTr("Classic Sepia Effect") 
                },
                Option {
                  	text: "Hitam Putih"
                  	description: qsTr("Black White Photograph effect")  
                },
                Option {
                    text: "Scribe"
                    description: qsTr("Yet another greyscale effect, different tone"); 
                },
                Option {
                    text: "Hike"
                    description: qsTr("Soar the color to the sky")  
                },
                Option {
                    text: "Cinta"
                    description: qsTr("Lovey?")  
                },
                Option {
                    text: "Analog"
                    description: qsTr("See your analog photograph")  
                },
                Option {
                    text: "Thermo"
                    description: qsTr("Warm temperature :3")  
                },
                Option {
                    text: "Desaku"
                    description: qsTr("Desaku yang kucinta~")  
                },
                Option {
                    text: "Kelud"
                    description: qsTr("Ashy Photo")  
                },
                Option {
                    text: "Hero"
                    description: qsTr("Be a man!")  
                },
                Option {
                    text: "Pencil"
                    description: qsTr("Sketching is fun")  
                },
                Option {
                    text: "Stylish"
                    description: qsTr("Be trendy")  
                },
                Option {
                    text: "Lord Kelvin"
                    description: qsTr("Yeah, it's Kelvin")  
                },
                Option {
                    text: "Hudson"
                    description: qsTr("Have a jenkins")  
                },
                Option {
                    text: "Pixelate"
                    description: qsTr("Fun Blocks")  
                }
            ]
            onSelectedIndexChanged: {
                compo.effect = ddEffect.selectedIndex;
            }
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight;
            }
            Button {
                text: qsTr("Pick")
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                onClicked: {
                    fpo.open();
                }
            }
            Button {
                text: qsTr("Save")
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                onClicked: {
                    fps.open();
                }
            }
        }
    }
    attachedObjects: [
        FilePicker {
            id:fpo
            type : FileType.Picture
            title : "Select Picture"
            onFileSelected : {
                console.log("FileSelected signal received : " + selectedFiles);
                compo.setImage(selectedFiles[0]);
            }
        },
        FilePicker {
            id: fps
            type: FileType.Picture
            mode: FilePickerMode.Saver
            title : "Save as"
            onFileSelected: {
                compo.saveResult(selectedFiles[0]);
                tst.body = selectedFiles[0] + " Saved!"
                tst.show()
            }
        },
        SystemToast {
            id: tst
            body: "Saved!"
        }
    ]
}
