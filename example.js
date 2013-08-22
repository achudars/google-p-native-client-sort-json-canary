function moduleDidLoad() {
  body = document.getElementsByTagName("body")[0];
  common.updateStatus('SUCCESS');
  body.className = "success";
  msg = document.getElementById('input2').value;
  common.naclModule.postMessage(msg);
  getExpectedResult();
}

function postThreadFunc(numThreads) {
  /*return function () {
    common.naclModule.postMessage({'message' : 'set_threads',
                                   'value' : numThreads});
  }*/
}

// Add event listeners after the NaCl module has loaded.  These listeners will
// forward messages to the NaCl module via postMessage()
function attachListeners() {
  document.getElementById('main-form').addEventListener('change',
    function() {
      common.updateStatus('BENCHMARKING... (please wait)');
      body = document.getElementsByTagName("body")[0];
      body.className = "loading";
      msg = document.getElementById('input2').value;
      common.naclModule.postMessage(msg);
      common.updateStatus('SUCCESS');
      body.className = "success";
      getExpectedResult();

    });
}

// Handle a message coming from the NaCl module.
function handleMessage(message_event) {
  document.getElementById('output').innerHTML = message_event.data;
}

var getExpectedResult = function() {
  var numbers = JSON.parse(Array(document.getElementById('input').value));

  function sortByValue(array, value) {
      return array.sort(function(a, b) {
          var x = a[value];
          var y = b[value];
          return ((x < y) ? -1 : ((x > y) ? 1 : 0));
      });
  }
  numbers = sortByValue(numbers, 'name');
  document.getElementById('expectedOutput').innerHTML = JSON.stringify(numbers);
};
