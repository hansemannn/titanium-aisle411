/**
 * Ti.iCloud
 * Copyright (c) 2017-Present by Axway Appcelerator
 * All Rights Reserved.
 */

'use strict';

exports.id = 'ti.icloud';
exports.cliVersion = '>=3.2';
exports.init = init;

/**
 * Main entry point for our plugin which looks for the platform specific
 * plugin to invoke
 */
function init(logger, config, cli, appc) {
	cli.on('build.ios.xcodeproject', {
		pre: function(data) {
			logger.info('Enabling Swift support ...');

			var xobjs = data.args[0].hash.project.objects;
			var SWIFT_VERSION = 3.1

			Object.keys(xobjs.PBXNativeTarget).forEach(function (targetUuid) {
				var target = xobjs.PBXNativeTarget[targetUuid];
				if (target && typeof target === 'object') {
					xobjs.XCConfigurationList[target.buildConfigurationList].buildConfigurations.forEach(function (buildConf) {
						var buildSettings = xobjs.XCBuildConfiguration[buildConf.value].buildSettings;
						buildSettings.ALWAYS_EMBED_SWIFT_STANDARD_LIBRARIES = 'YES';

						if (!buildSettings.SWIFT_VERSION) {
							buildSettings.SWIFT_VERSION = SWIFT_VERSION;
						}

						// LD_RUNPATH_SEARCH_PATHS is a space separated string of paths
						var searchPaths = '';
						if(buildSettings.LD_RUNPATH_SEARCH_PATHS && buildSettings.LD_RUNPATH_SEARCH_PATHS.length){
							for(var ii=0; ii < buildSettings.LD_RUNPATH_SEARCH_PATHS.length; ii++) {
								searchPaths += ' ' + buildSettings.LD_RUNPATH_SEARCH_PATHS[ii].replace(/"/g, '').replace(/\\/g, '');
							}
						}

						if (searchPaths.indexOf('$(inherited)') === -1) {
							searchPaths += ' $(inherited)';
						}
						if (searchPaths.indexOf('@executable_path/Frameworks') === -1) {
							searchPaths += ' @executable_path/Frameworks';
						}

						buildSettings.LD_RUNPATH_SEARCH_PATHS = '"' + searchPaths.trim() + '"';
					});
				}
			});
		}
	});
}
