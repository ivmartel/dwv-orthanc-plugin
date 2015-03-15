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
#include <stdio.h>
#include <sstream> 

#include <EmbeddedResources.h>

static OrthancPluginContext* context = NULL;

ORTHANC_PLUGINS_API int32_t CallbackRessources( 
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
    return -1;
  }
  // get the answer buffer
  const char* answer = static_cast<const char*>(Orthanc::EmbeddedResources::GetDirectoryResourceBuffer(
    Orthanc::EmbeddedResources::DWV_EXPLORER, path.c_str()));
  // find mime type
  const std::string ext = file.substr( file.find_last_of('.') + 1, file.size() );
  std::string mimeType = "";
  if ( ext == "html" ) {
    mimeType = "text/html";
  }
  else if ( ext == "css" ) {
    mimeType = "text/css";
  }
  else if ( ext == "js" ) {
    mimeType = "text/javascript";
  }
  // send the answer
  OrthancPluginAnswerBuffer(context, output, answer, answerSize, mimeType.c_str());
  // all good
  return 0;
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
  return "0.3";
}

} // extern "C"
