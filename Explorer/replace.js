// OrthancExplorer/explorer.js@L145
// -> made it async since "Synchronous XMLHttpRequest on the main thread is deprecated".
function MyGetResource(uri, callback)
{
  $.ajax({
    url: '..' + uri,
    dataType: 'json',
    async: true,
    cache: false,
    success: function(s) {
      callback(s);
    }
  });
}

$('#instance').live('pagebeforecreate', function() {
  $('#instance-preview').parent().remove();
  var b = $('<a>')
    .attr('data-role', 'button')
    .attr('href', '#')
    .attr('data-icon', 'search')
    .attr('data-theme', 'e')
    .text('Instance viewer');

  b.insertBefore($('#instance-delete').parent().parent());
  b.click(function() {
    if ($.mobile.pageData) {
      MyGetResource('/instances/' + $.mobile.pageData.uuid + '/frames', function(frames) {
        if (frames.length == 1)
        {
          // Viewing a single-frame image
          var instanceUri = window.location.protocol + '//' + window.location.host;
          instanceUri += '/instances/' + $.mobile.pageData.uuid + '/file';
          var dwvUri = '../dwv-plugin/dwv/viewers/mobile/index.html';
          // form input element 
          var input = document.createElement("input");
          input.type = "text";
          input.name = "urls";
          input.value = instanceUri;
          // form to send data via POST
          var form = document.createElement("form");
          form.method = "post";
          form.action = dwvUri;
          //form.target = "_self"; // display in same frame
          form.target = "_blank"; // display in new window or tab
          form.appendChild(input);
          document.body.appendChild(form);
          form.submit();
        }
        else
        {
          // Viewing a multi-frame image
          var images = [];
          for (var i = 0; i < frames.length; i++) {
            images.push([ '../instances/' + $.mobile.pageData.uuid + '/frames/' + i + '/preview' ]);
          }
          jQuery.slimbox(images, 0, {
            overlayFadeDuration : 1,
            resizeDuration : 1,
            imageFadeDuration : 1,
            loop : true
          });
        }
      });
    }
  });
});

$('#series').live('pagebeforecreate', function() {
  $('#series-preview').parent().remove();
  var b = $('<a>')
    .attr('data-role', 'button')
    .attr('href', '#')
    .attr('data-icon', 'search')
    .attr('data-theme', 'e')
    .text('Series viewer');

  b.insertBefore($('#series-delete').parent().parent());
  b.click(function() {
    if ($.mobile.pageData) {
      MyGetResource('/series/' + $.mobile.pageData.uuid, function(series) {
        // (yves) removed this asynchronous call since it blocks window.open.
        // Can the series.Instances[#] be different than instances.ID??
        //GetMultipleResources('instances', series.Instances, function(instances) {
        var instances = series.Instances;
        Sort(instances, function(x) { return x.IndexInSeries; }, true, false);
        var rootUri = window.location.protocol + '//' + window.location.host + '/instances/';
        var instancesUri = "";
        for (var i = 0; i < instances.length; i++) {
          if ( i > 0 ) instancesUri += ',';
          instancesUri += rootUri + instances[i] + '/file';
        }
        var dwvUri = '../dwv-plugin/dwv/viewers/mobile/index.html';
        // form input element 
        var input = document.createElement("input");
        input.type = "text";
        input.name = "urls";
        input.value = instancesUri;
        // form to send data via POST
        var form = document.createElement("form");
        form.method = "post";
        form.action = dwvUri;
        //form.target = "_self"; // display in same frame
        form.target = "_blank"; // display in new window or tab
        form.appendChild(input);
        document.body.appendChild(form);
        form.submit();
      });
    }
  });
});
