Purpose of this file is to explain two different components for RW61x platform:
	- component.els_pkc.platform.rw61x_standalone_clib_gdet_sensor:
		- This component only provides gdet sensor related interface of clib. 
		  It shall only be used within default examples of RW61x where entire clib-interface is not required.
	- component.els_pkc.platform.rw61x:
		- This component includes all features supported by Clib for RW61x platform.