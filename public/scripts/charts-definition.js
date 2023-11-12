// Create the charts when the web page loads
window.addEventListener('load', onload);

function onload(event) {
  chartT = createTemperatureChart();
  chartH = createHumidityChart();
  chartP = createPressureChart();
}

// Create Temperature Chart
function createTemperatureChart() {
  var chart = new Highcharts.Chart({
    chart: {
      renderTo: 'chart-temperature',
      type: 'spline',
    },
    series: [
      {
        name: 'BME280',
      },
    ],
    title: {
      text: undefined,
    },
    plotOptions: {
      line: {
        animation: false,
        dataLabels: {
          enabled: true,
        },
      },
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' },
    },
    yAxis: {
      title: {
        text: 'Temperature Celsius Degrees',
      },
    },
    credits: {
      enabled: false,
    },
  });
  return chart;
}
