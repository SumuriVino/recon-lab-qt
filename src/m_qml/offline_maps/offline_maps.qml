import QtQuick 2.0

import QtLocation 5.6
import QtPositioning 5.6

Rectangle
{
    width  : 512
    height : 512
    visible: true

    //default location
    property variant location: QtPositioning.coordinate(0.00, 0.00)

    Plugin {
                id: mapPlugin
                name: 'osm';
                PluginParameter {
                    name: 'osm.mapping.cache.disk.size'
                    value: '1'
                }

                PluginParameter {
                    name: 'osm.mapping.offline.directory'
                    value: '../Resources/graphics/offline_maps'
                }
                PluginParameter {
                    name: 'osm.mapping.providersrepository.address'
                    value: '/home/khalnayak/Document/abc'
                }
            }

    Map {
        id: map
        anchors.fill: parent

        activeMapType: map.supportedMapTypes[2] //value 1 for online and 2 for offline

        zoomLevel: 8
        center: location
        plugin: mapPlugin

        MapQuickItem {
        id: marker

        sourceItem: Image {
            id: image
            source: "marker.png"
        }

        anchorPoint.x: image.width/4
        anchorPoint.y: image.height
        coordinate: location

        }
    }
}
