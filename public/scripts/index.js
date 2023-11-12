// convert epochtime to JavaScripte Date object
function epochToJsDate(epochTime) {
  return new Date(epochTime * 1000);
}

// convert time to human-readable format YYYY/MM/DD HH:MM:SS
function epochToDateTime(epochTime) {
  var epochDate = new Date(epochToJsDate(epochTime));
  var dateTime =
    epochDate.getFullYear() +
    '/' +
    ('00' + (epochDate.getMonth() + 1)).slice(-2) +
    '/' +
    ('00' + epochDate.getDate()).slice(-2) +
    ' ' +
    ('00' + epochDate.getHours()).slice(-2) +
    ':' +
    ('00' + epochDate.getMinutes()).slice(-2) +
    ':' +
    ('00' + epochDate.getSeconds()).slice(-2);

  return dateTime;
}

// function to plot values on charts
function plotValues(chart, timestamp, value) {
  var x = epochToJsDate(timestamp).getTime();
  var y = Number(value);
  if (chart.series[0].data.length > 40) {
    chart.series[0].addPoint([x, y], true, true, true);
  } else {
    chart.series[0].addPoint([x, y], true, false, true);
  }
}

// DOM elements
const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector('#content-sign-in');
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector('#authentication-bar');
const deleteButtonElement = document.getElementById('delete-button');
const deleteModalElement = document.getElementById('delete-modal');
const deleteDataFormElement = document.querySelector('#delete-data-form');
const viewDataButtonElement = document.getElementById('view-data-button');
const hideDataButtonElement = document.getElementById('hide-data-button');
const tableContainerElement = document.querySelector('#table-container');
const chartsRangeInputElement = document.getElementById('charts-range');
const loadDataButtonElement = document.getElementById('load-data');
const cardsCheckboxElement = document.querySelector('input[name=cards-checkbox]');
const gaugesCheckboxElement = document.querySelector('input[name=gauges-checkbox]');
const chartsCheckboxElement = document.querySelector('input[name=charts-checkbox]');

// Elements for sensor readings
const tmpElement = document.getElementById('tmp');
const lvlElement = document.getElementById('lvl');
const flwElement = document.getElementById('flw');
const turElement = document.getElementById('tur');
const autElement = document.getElementById('unact');
const autElementActive = document.getElementById('act');
const clnElement = document.getElementById('uncln');
const clnElementActive = document.getElementById('cln');
const filElement = document.getElementById('unfil');
const filElementActive = document.getElementById('fil');
const cardsReadingsElement = document.querySelector('#cards-div');
const gaugesReadingsElement = document.querySelector('#gauges-div');
const chartsDivElement = document.querySelector('#charts-div');
const presElement = document.getElementById('pres');
const updateElement = document.getElementById('lastUpdate');

// Indicator elements
const waterLevelIndicator = document.getElementById('water-level-indicator');
const dirtyWaterIndicator = document.getElementById('dirty-water-indicator');

// Define the threshold
const waterLevelThreshold = 20; // Change this to your desired threshold value
const turbidityThreshold = 25; // Change this to your desired threshold value

// MANAGE LOGIN/LOGOUT UI
const setupUI = (user) => {
  if (user) {
    //toggle UI elements
    loginElement.style.display = 'none';
    contentElement.style.display = 'block';
    authBarElement.style.display = 'block';
    userDetailsElement.style.display = 'block';
    userDetailsElement.innerHTML = user.email;

    // get user UID to get data from database
    var uid = user.uid;
    console.log(uid);

    // Database paths (with user UID)
    var dbPathTmp = 'UsersData/' + uid.toString() + '/temperature';
    var dbPathLvl = 'UsersData/' + uid.toString() + '/water-level';
    var dbPathFlw = 'UsersData/' + uid.toString() + '/water-flow';
    var dbPathTur = 'UsersData/' + uid.toString() + '/turbidity';
    var dbPathAut = 'UsersData/' + uid.toString() + '/automation';
    var dbPathCln = 'UsersData/' + uid.toString() + '/cleaning-state';
    var dbPathFil = 'UsersData/' + uid.toString() + '/filling-state';
    var dbPathLog = 'UsersData/' + uid.toString() + '/log';
    var chartPath = 'UsersData/' + uid.toString() + '/charts/range';

    // Database references
    var dbRefTmp = firebase.database().ref().child(dbPathTmp);
    var dbRefLvl = firebase.database().ref().child(dbPathLvl);
    var dbRefFlw = firebase.database().ref().child(dbPathFlw);
    var dbRefTur = firebase.database().ref().child(dbPathTur);
    var dbRefAut = firebase.database().ref().child(dbPathAut);
    var dbRefCln = firebase.database().ref().child(dbPathCln);
    var dbRefFil = firebase.database().ref().child(dbPathFil);
    var dbRefLog = firebase.database().ref(dbPathLog);
    var chartRef = firebase.database().ref(chartPath);
    
    // Switch on/off
    var autStatus;
    var clnStatus;
    var filStatus;

    // Update page with new readings
    dbRefTmp.on('value', (snap) => {
      tmpElement.innerText = snap.val().toFixed(2);
    });

    dbRefLvl.on('value', (snap) => {
      lvlElement.innerText = snap.val().toFixed(2);
    });

    dbRefFlw.on('value', (snap) => {
      flwElement.innerText = snap.val().toFixed(2);
    });

    dbRefTur.on('value', (snap) => {
      turElement.innerText = snap.val().toFixed(2);
    });

    // Automation state
    dbRefAut.on('value', (snap) => {
      autStatus = snap.val();
      console.log(autStatus);
      if (autStatus == 1) {
        autElement.style.display = 'none';
        autElementActive.style.display = 'block';
      } else {
        autElement.style.display = 'block';
        autElementActive.style.display = 'none';
      }
    });

    const setActive = () => {
      if (autStatus == 1) {
        dbRefAut.set(0);
        autStatus = 0;
      } else {
        dbRefAut.set(1);
        autStatus = 1;
      }
    };

    if (autElement) autElement.addEventListener('click', setActive);

    const setNonActive = () => {
      if (autStatus == 0) {
        dbRefAut.set(1);
        autStatus = 1;
      } else {
        dbRefAut.set(0);
        autStatus = 0;
      }
    };
    if (autElementActive)
      autElementActive.addEventListener('click', setNonActive);

    // Cleaning state
    dbRefCln.on('value', (snap) => {
      clnStatus = snap.val();
      console.log(clnStatus);
      if (clnStatus == 1) {
        clnElement.style.display = 'none';
        clnElementActive.style.display = 'block';
      } else {
        clnElement.style.display = 'block';
        clnElementActive.style.display = 'none';
      }
    });

    const setClean = () => {
      if (clnStatus == 1) {
        dbRefCln.set(0);
        clnStatus = 0;
      } else {
        dbRefCln.set(1);
        clnStatus = 1;
      }
      // Turn off the toggle after x seconds (x000 milliseconds)
      setTimeout(turnOffCleaningToggle, 5000);
    };

    // Function to turn off the cleaning toggle
    const turnOffCleaningToggle = () => {
      dbRefCln.set(0);
      clnStatus = 0;
    };

    if (clnElement) clnElement.addEventListener('click', setClean);

    const setNoClean = () => {
      if (clnStatus == 0) {
        dbRefCln.set(1);
        clnStatus = 1;
      } else {
        dbRefCln.set(0);
        clnStatus = 0;
      }
    };
    if (clnElementActive)
      clnElementActive.addEventListener('click', setNoClean);

    //Filling State
    dbRefFil.on('value', (snap) => {
      filStatus = snap.val();
      console.log(filStatus);
      if (filStatus == 1) {
        filElement.style.display = 'none';
        filElementActive.style.display = 'block';
      } else {
        filElement.style.display = 'block';
        filElementActive.style.display = 'none';
      }
    });

    const setFill = () => {
      if (filStatus == 1) {
        dbRefFil.set(0);
        filStatus = 0;
      } else {
        dbRefFil.set(1);
        filStatus = 1;
      }

      // Turn off the toggle after x seconds (x000 milliseconds)
      setTimeout(turnOffFillingToggle, 5000);
    };

    // Function to turn off the cleaning toggle
    const turnOffFillingToggle = () => {
      dbRefFil.set(0);
      filStatus = 0;
    };

    if (filElement) filElement.addEventListener('click', setFill);

    const setNoFill = () => {
      if (filStatus == 0) {
        dbRefFil.set(1);
        filStatus = 1;
      } else {
        dbRefFil.set(0);
        filStatus = 0;
      }
    };
    if (filElementActive) filElementActive.addEventListener('click', setNoFill);

    // Water level indicator
    dbRefLvl.on('value', (snap) => {
      const waterLevel = snap.val().toFixed(2);
      lvlElement.innerText = waterLevel;

      // Change the indicator's color based on the water level
      if (waterLevel < waterLevelThreshold) {
        // Set the indicator's color to red
        waterLevelIndicator.style.backgroundColor = 'red';
      } else {
        // Set the indicator's color to green (or any other color)
        waterLevelIndicator.style.backgroundColor = 'grey';
      }

      // Turbidity indicator
      dbRefTur.on('value', (snap) => {
        const turbidity = snap.val().toFixed(2);
        turElement.innerText = turbidity;

        // Change the dirty water indicator's color based on turbidity
        if (turbidity > turbidityThreshold) {
          dirtyWaterIndicator.style.backgroundColor = 'red';
        } else {
          dirtyWaterIndicator.style.backgroundColor = 'grey';
        }
      });
    });

    // if user is logged out
  } else {
    // toggle UI elements
    loginElement.style.display = 'block';
    authBarElement.style.display = 'none';
    userDetailsElement.style.display = 'none';
    contentElement.style.display = 'none';
  }
};
