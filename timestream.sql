SELECT time, 
      MAX(CASE WHEN measure_name = 'gyro' THEN measure_value::varchar END) "gyroXYZ",
      MAX(CASE WHEN measure_name = 'acc' THEN measure_value::varchar END) "accXYZ",
      MAX(CASE WHEN measure_name = 'ahrs' THEN measure_value::varchar END) "ahrsPRY"
FROM "IMU"."data" 
WHERE measure_name != 'sensor' 
GROUP BY 1
ORDER BY time DESC 
LIMIT 10 
