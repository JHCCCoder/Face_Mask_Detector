var myChart = echarts.init(document.getElementById('main'));
var int1 = self.setInterval("timers1()", 50)

function timers1() {
    // $.ajax({
    //     url: '/api1',
    //     type: 'get',
    //     success: function (data) {
            option = {
                tooltip: {
                    trigger: 'axis',
                    position: function (pt) {
                        return [pt[0], '10%'];
                    }
                },
                title: {
                    left: 'center',
                    text: 'Face Status'
                },
                toolbox: {
                    feature: {
                        dataZoom: {
                            yAxisIndex: 'none'
                        },
                        restore: {},
                        saveAsImage: {}
                    }
                },
                xAxis: {
                    type: 'time',
                    boundaryGap: false
                },
                yAxis: {
                    type: 'value',
                    boundaryGap: [0, '100%']
                },
                dataZoom: [
                    {
                        type: 'inside',
                        start: 0,
                        end: 100
                    },
                    {
                        start: 0,
                        end: 100
                    }
                ],
                series: [
                    {
                        name: 'Price Data',
                        type: 'line',
                        smooth: true,
                        symbol: 'none',
                        areaStyle: {},
                        data: data.data2
                    }
                ]
            };
            myChart.setOption(option);
        // }
    // })
}
//
// var myChart2 = echarts.init(document.getElementById('main2'));
// var int2 = self.setInterval("timers2()", 50)
//
// function timers2() {
//     $.ajax({
//         url: '/api2',
//         type: 'get',
//         success: function (data) {
//             option2 = {
//                 title: {
//                     text: 'take rate',
//                     subtext: 'rate',
//                     left: 'center'
//                 },
//                 tooltip: {
//                     trigger: 'item'
//                 },
//                 legend: {
//                     orient: 'vertical',
//                     left: 'left'
//                 },
//                 series: [
//                     {
//                         name: 'count',
//                         type: 'pie',
//                         radius: '50%',
//                         data: data.data,
//                         emphasis: {
//                             itemStyle: {
//                                 shadowBlur: 10,
//                                 shadowOffsetX: 0,
//                                 shadowColor: 'rgba(0, 0, 0, 0.5)'
//                             }
//                         }
//                     }
//                 ]
//             };
//             myChart2.setOption(option2);
//         }
//     })
// }
//
// var int = self.setInterval("timers()", 50)
//
// function timers() {
//     $.ajax({
//         url: '/api3',
//         type: 'get',
//         success: function (data) {
//             console.log(1)
//             $('#tupian img').attr('src', data.src);
//         }
//     })
// }
//
// var int4 = self.setInterval("timers4()", 50)
//
// function timers4() {
//     $.ajax({
//         url: '/api4',
//         type: 'get',
//         success: function (data) {
//             $('#m1').text(data.r1);
//             $('#m0').text(data.r0);
//         }
//     })
// }
//
//
//
//
//
//
// var int12 = self.setInterval("timers4()", 50)
// function timers4() {
//     $.ajax({
//         url: '/api4',
//         type: 'get',
//         success: function (data) {
//             $('#m1').text(data.r1);
//             $('#m0').text(data.r0);
//             $('#m3').text(data.t);
//             $('#m4').text(data.route);
//
//         }
//     })
// }

$(document).ready(function() {
    var myChart2 = echarts.init(document.getElementById('barChart'));
    var myChart = echarts.init(document.getElementById('main'));
    // barchart
    var data = {
        'data':
            [
                {'value': 12, 'name': 'unmask'},
                {'value': 45, 'name': 'mask'}
            ]
    }

    option2 = {
        title: {
            text: 'take rate',
            subtext: 'rate',
            left: 'center'
        },
        tooltip: {
            trigger: 'item'
        },
        legend: {
            orient: 'vertical',
            left: 'left'
        },
        series: [
            {
                name: 'count',
                type: 'pie',
                radius: '80%',
                data: data.data,
                emphasis: {
                    itemStyle: {
                        shadowBlur: 10,
                        shadowOffsetX: 0,
                        shadowColor: 'rgba(0, 0, 0, 0.5)'
                    }
                }
            }
        ]
    };
    myChart2.setOption(option2);


    var dataMain = {
        'data':
            [
                {'value': 12, 'name': 'unmask'},
                {'value': 45, 'name': 'mask'}
            ]
    }
    // main chart
    option = {
        tooltip: {
            trigger: 'axis',
            position: function (pt) {
                return [pt[0], '10%'];
            }
        },
        title: {
            left: 'center',
            text: 'Face Status'
        },
        toolbox: {
            feature: {
                dataZoom: {
                    yAxisIndex: 'none'
                },
                restore: {},
                saveAsImage: {}
            }
        },
        xAxis: {
            type: 'time',
            boundaryGap: false
        },
        yAxis: {
            type: 'value',
            boundaryGap: [0, '100%']
        },
        dataZoom: [
            {
                type: 'inside',
                start: 0,
                end: 100
            },
            {
                start: 0,
                end: 100
            }
        ],
        series: [
            {
                name: 'Price Data',
                type: 'line',
                smooth: true,
                symbol: 'none',
                areaStyle: {},
                data: dataMain.data
            }
        ]
    };
    myChart.setOption(option);
})
