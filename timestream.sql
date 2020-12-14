SELECT time,
     MAX(CASE WHEN measure_name = 'gyroX' THEN measure_value::double END) "gyroX",
     MAX(CASE WHEN measure_name = 'gyroY' THEN measure_value::double END) "gyroY",
     MAX(CASE WHEN measure_name = 'gyroZ' THEN measure_value::double END) "gyroZ",
     MAX(CASE WHEN measure_name = 'accX' THEN measure_value::double END) "accX",
     MAX(CASE WHEN measure_name = 'accY' THEN measure_value::double END) "accY",
     MAX(CASE WHEN measure_name = 'accZ' THEN measure_value::double END) "accZ",
     MAX(CASE WHEN measure_name = 'pitch' THEN measure_value::double END) "pitch",
     MAX(CASE WHEN measure_name = 'roll' THEN measure_value::double END) "roll",
     MAX(CASE WHEN measure_name = 'yaw' THEN measure_value::double END) "yaw"
FROM "IMU"."data"
GROUP BY 1
ORDER BY time DESC
LIMIT 10