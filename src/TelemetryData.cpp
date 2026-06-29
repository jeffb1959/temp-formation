#include "TelemetryData.h"

void printTelemetryJson(const TelemetryData& data, Print& output) {
  output.print('{');
  output.print("\"device_id\":\"");
  output.print(data.device_id);
  output.print("\",");
  output.print("\"device_name\":\"");
  output.print(data.device_name);
  output.print("\",");
  output.print("\"sensor_type\":\"");
  output.print(data.sensor_type);
  output.print("\",");
  output.print("\"temperature_c\":");
  output.print(data.temperature_c, 2);
  output.print(",");
  output.print("\"local_status\":\"");
  output.print(TemperatureStatus::toString(data.local_status));
  output.print("\",");
  output.print("\"reading_interval_s\":");
  output.print(data.reading_interval_s);
  output.print(",");
  output.print("\"timestamp_ms\":");
  output.print(data.timestamp_ms);
  output.println('}');
}
