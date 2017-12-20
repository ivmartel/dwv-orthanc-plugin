/**
 * Copyright (C) 2014 Yves Martelli
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include <orthanc/OrthancCPlugin.h>

#include <string>
#include <vector>
#include <stdio.h>
#include <sstream> 

#include <EmbeddedResources.h>

#if ( (ORTHANC_PLUGINS_MINIMAL_MAJOR_NUMBER == 0 && \
  ORTHANC_PLUGINS_MINIMAL_MINOR_NUMBER == 8 && \
  ORTHANC_PLUGINS_MINIMAL_REVISION_NUMBER == 6) || \
  (ORTHANC_PLUGINS_MINIMAL_MAJOR_NUMBER <= 0 && \
  ORTHANC_PLUGINS_MINIMAL_MINOR_NUMBER <= 9 && \
  ORTHANC_PLUGINS_MINIMAL_REVISION_NUMBER <= 4) )
#  define RETURN_TYPE     int32_t
#  define RETURN_SUCCESS  0
#  define RETURN_FAILURE  -1
#else
#  define RETURN_TYPE     OrthancPluginErrorCode
#  define RETURN_SUCCESS  OrthancPluginErrorCode_Success
#  define RETURN_FAILURE  OrthancPluginErrorCode_Plugin
#endif

static OrthancPluginContext* context = NULL;

ORTHANC_PLUGINS_API RETURN_TYPE CallbackRessources( 
  OrthancPluginRestOutput* output,
  const char* url,
  const OrthancPluginHttpRequest* request)
{
  // build the file path
  const std::string file = request->groups[0];
  const std::string path = "/" + file;
  // get the answer size from the resources
  uint32_t answerSize = 0;
  try
  {
    answerSize = Orthanc::EmbeddedResources::GetDirectoryResourceSize(
      Orthanc::EmbeddedResources::DWV_EXPLORER, path.c_str());
  }
  catch (...)
  {
    std::ostringstream oss;
    oss << "Unable to find HTTP resource: " << path;
    OrthancPluginLogError(context, oss.str().c_str());
    return RETURN_FAILURE;
  }
  // get the answer buffer
  const char* answer = static_cast<const char*>(Orthanc::EmbeddedResources::GetDirectoryResourceBuffer(
    Orthanc::EmbeddedResources::DWV_EXPLORER, path.c_str()));
  // find mime type
  const std::string ext = file.substr( file.find_last_of('.') + 1, file.size() );
  std::string mimeType = "";
  if ( ext == "html" ) {
    mimeType = "text/html";
    // cast answer to string
    std::string page = answer;
    if (request->body != NULL) {
      // comment out default applauncher
      const std::string applau = "<script type=\"text/javascript\" src=\"applauncher.js\"></script>";
      const std::size_t foundApplau = page.find(applau);
      if (foundApplau != std::string::npos) {
        page.insert(foundApplau + applau.size(), "-->");
        page.insert(foundApplau, "<!--");
      }
      // insert own applauncher
      const std::size_t foundHead = page.find("</head>");
      if (foundHead != std::string::npos) {
        // split urls
        const std::string body = request->body;
        std::string urls = body.substr(5); // remove prefix ('urls=')
        std::vector< std::string > urlVec;
        const std::string comma = "%2C"; // ',' url encoded
        std::size_t foundComma = urls.find(comma);
        while (foundComma != std::string::npos) {
          urlVec.push_back(urls.substr(0, foundComma));
          urls = urls.substr(foundComma + comma.size());
          foundComma = urls.find(comma);
        }
        // last one or for only one case
        if (foundComma == std::string::npos) {
          urlVec.push_back(urls);
        }
        // launcher string
        std::string launch = "<script type=\"text/javascript\">\n";
        launch += "// start app function\n";
        launch += "function startApp() {\n";
        launch += "    // translate page\n";
        launch += "    dwv.i18nPage();\n";
        launch += "    // main application\n";
        launch += "    var myapp = new dwv.App();\n";
        launch += "    // initialise the application\n";
        launch += "    var options = {\n";
        launch += "        \"containerDivId\": \"dwv\",\n";
        launch += "        \"fitToWindow\": true,\n";
        launch += "        \"gui\": [\"tool\", \"load\", \"help\", \"undo\", \"version\", \"tags\", \"drawList\"],\n";
        launch += "        \"loaders\": [\"File\", \"Url\"],\n";
        launch += "        \"tools\": [\"Scroll\", \"WindowLevel\", \"ZoomAndPan\", \"Draw\", \"Livewire\", \"Filter\", \"Floodfill\"],\n";
        launch += "        \"filters\": [\"Threshold\", \"Sharpen\", \"Sobel\"],\n";
        launch += "        \"shapes\": [\"Arrow\", \"Ruler\", \"Protractor\", \"Rectangle\", \"Roi\", \"Ellipse\", \"FreeHand\"],\n";
        launch += "        \"isMobile\": true\n";
        launch += "        //\"defaultCharacterSet\": \"chinese\"\n";
        launch += "    };\n";
        launch += "    if ( dwv.browser.hasInputDirectory() ) {\n";
        launch += "        options.loaders.splice(1, 0, \"Folder\");\n";
        launch += "    }\n";
        launch += "    myapp.init(options);\n";
        launch += "\n";

        launch += "   myapp.loadURLs([\n";
        for (unsigned int i = 0; i < urlVec.size(); ++i) {
          if (i != 0) launch += ",\n";
          launch += "      decodeURIComponent(\"" + urlVec[i] + "\")";
        }
        launch += "\n";
        launch += "   ]);\n";

        launch += "    var size = dwv.gui.getWindowSize();\n";
        launch += "    $(\".layerContainer\").height(size.height);\n";
        launch += "}\n";
        launch += "\n";
        launch += "// Image decoders (for web workers)\n";
        launch += "dwv.image.decoderScripts = {\n";
        launch += "    \"jpeg2000\": \"../../decoders/pdfjs/decode-jpeg2000.js\",\n";
        launch += "    \"jpeg-lossless\": \"../../decoders/rii-mango/decode-jpegloss.js\",\n";
        launch += "    \"jpeg-baseline\": \"../../decoders/pdfjs/decode-jpegbaseline.js\"\n";
        launch += "};\n";
        launch += "\n";
        launch += "// status flags\n";
        launch += "var domContentLoaded = false;\n";
        launch += "var i18nInitialised = false;\n";
        launch += "// launch when both DOM and i18n are ready\n";
        launch += "function launchApp() {\n";
        launch += "    if ( domContentLoaded && i18nInitialised ) {\n";
        launch += "        startApp();\n";
        launch += "    }\n";
        launch += "}\n";
        launch += "// i18n ready?\n";
        launch += "dwv.i18nOnInitialised( function () {\n";
        launch += "    // call next once the overlays are loaded\n";
        launch += "    var onLoaded = function (data) {\n";
        launch += "        dwv.gui.info.overlayMaps = data;\n";
        launch += "        i18nInitialised = true;\n";
        launch += "        launchApp();\n";
        launch += "    };\n";
        launch += "    // load overlay map info\n";
        launch += "    $.getJSON( dwv.i18nGetLocalePath(\"overlays.json\"), onLoaded )\n";
        launch += "    .fail( function () {\n";
        launch += "        console.log(\"Using fallback overlays.\");\n";
        launch += "        $.getJSON( dwv.i18nGetFallbackLocalePath(\"overlays.json\"), onLoaded );\n";
        launch += "    });\n";
        launch += "});\n";
        launch += "\n";
        launch += "// check browser support\n";
        launch += "dwv.browser.check();\n";
        launch += "// initialise i18n\n";
        launch += "dwv.i18nInitialise();\n";
        launch += "\n";
        launch += "// DOM ready?\n";
        launch += "$(document).ready( function() {\n";
        launch += "    domContentLoaded = true;\n";
        launch += "    launchApp();\n";
        launch += "});\n";

        launch += "</script>\n";
        // insert launcher into page
        page.insert(foundHead, launch);
      }
    }
    // send the answer
    OrthancPluginAnswerBuffer(context, output, page.c_str(), page.size(), mimeType.c_str());
  }
  else {
    if (ext == "css") {
      mimeType = "text/css";
    }
    else if (ext == "js") {
      mimeType = "text/javascript";
    }
    // send the answer
    OrthancPluginAnswerBuffer(context, output, answer, answerSize, mimeType.c_str());
  }
  // all good
  return RETURN_SUCCESS;
}

extern "C"
{

ORTHANC_PLUGINS_API int32_t OrthancPluginInitialize(OrthancPluginContext* c)
{
  char info[1024];

  context = c;
  OrthancPluginLogWarning(context, "DWV plugin is initializing");

  // Check the version of the Orthanc core
  if (OrthancPluginCheckVersion(c) == 0)
  {
    sprintf(info, "Your version of Orthanc (%s) must be above %d.%d.%d to run this plugin",
            c->orthancVersion,
            ORTHANC_PLUGINS_MINIMAL_MAJOR_NUMBER,
            ORTHANC_PLUGINS_MINIMAL_MINOR_NUMBER,
            ORTHANC_PLUGINS_MINIMAL_REVISION_NUMBER);
    OrthancPluginLogError(context, info);
    return -1;
  }

  // Register the callbacks
  OrthancPluginRegisterRestCallback(context, "/dwv-plugin/(.*)", CallbackRessources);
  
  // Replace preview button
  const std::string path = "/replace.js";
  const char* answer = static_cast<const char*>(Orthanc::EmbeddedResources::GetDirectoryResourceBuffer(
    Orthanc::EmbeddedResources::DWV_EXPLORER, path.c_str()));
  OrthancPluginExtendOrthancExplorer(context, answer);

  return 0;
}


ORTHANC_PLUGINS_API void OrthancPluginFinalize()
{
  OrthancPluginLogWarning(context, "DWV plugin is finalizing");
}


ORTHANC_PLUGINS_API const char* OrthancPluginGetName()
{
  return "dwv-plugin";
}


ORTHANC_PLUGINS_API const char* OrthancPluginGetVersion()
{
  return "0.7";
}

} // extern "C"
