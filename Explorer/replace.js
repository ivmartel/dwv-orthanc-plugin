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
      GetSingleResource('instances', $.mobile.pageData.uuid + '/frames', function(frames) {
        if (frames.length == 1)
        {
          // Viewing a single-frame image
          var instanceUri = window.location.protocol + '//' + window.location.host;
          instanceUri += '/instances/' + $.mobile.pageData.uuid + '/file';
          var dwvUri = '../plugin-dwv/dwv/viewers/mobile';
          var fullUri = dwvUri + '/index.html?input=' + encodeURIComponent(instanceUri);
          window.open(fullUri, '_blank');
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
      GetSingleResource('series', $.mobile.pageData.uuid, function(series) {
        // (yves) removed this asynchronous call since it blocks window.open.
        // Can the series.Instances[#] be different than instances.ID??
        //GetMultipleResources('instances', series.Instances, function(instances) {
        var instances = series.Instances;
        Sort(instances, function(x) { return x.IndexInSeries; }, true, false);
        var instancesUri = window.location.protocol + '//' + window.location.host + '/instances/';
        if ( instances.length === 1 ) {
          instancesUri += instances[0] + '/file';
        }
        else {
          instancesUri += '?'; // dwv multiple uri keyword
          for (var i = 0; i < instances.length; i++) {
            if ( i > 0 ) instancesUri += '&';
            instancesUri += 'file=' + instances[i] + '/file';
          }
        }
        var dwvUri = '../plugin-dwv/dwv/viewers/mobile';
        var fullUri = dwvUri + '/index.html?input=' + encodeURIComponent(instancesUri);
        fullUri += '&dwvReplaceMode=void';
        window.open(fullUri, '_blank');
      });
    }
  });
});
