#ifndef TEMPERATURE_HUMIDITY_SENSOR_GROUP_H
#define TEMPERATURE_HUMIDITY_SENSOR_GROUP_H
#include <movingAvg.h>
#include <Array.h>
#include "../Sensor/Sensor.h"

namespace GrowController
{

  template <typename SensorType, int MAX_SENSORS = 8>

  class TemperatureHumiditySensorGroup
  {

  public:
    TemperatureHumiditySensorGroup(
        int tcaAddresses[],
        int sensorCount,
        int movingAverageCount = 200) : movingAverage(movingAverageCount)
    {
      this->sensorCount = sensorCount;
      this->movingAverage.begin();

      int total = min(MAX_SENSORS, sensorCount);
      for (int i = 0; i < total; i++)
      {
        SensorType sensor(tcaAddresses[i]);
        this->sensors[i] = sensor;
      }
    }

    update()
    {
      for (int i = 0; i < this->sensorCount; i++)
      {
        this->sensors[i].update();
      }
    }

    float getTemperature()
    {
      return getSafeAverageValue(&TemperatureHumiditySensorGroup::getSensorTemp);
    }

    float getHumidity()
    {
      return getSafeAverageValue(&TemperatureHumiditySensorGroup::getSensorHumidity);
    }

    float getVPD()
    {
      return this->getSafeAverageValue(&TemperatureHumiditySensorGroup::getSensorVPD);
    }

    float getTemp()
    {
      return this->getSafeAverageValue(&TemperatureHumiditySensorGroup::getSensorTemp);
    }

    float getAverageTemp()
    {
      return this->getSafeAverageValue(&TemperatureHumiditySensorGroup::getSensorAverageTemp);
    }

    float getAverageHumidity()
    {
      return this->getSafeAverageValue(&TemperatureHumiditySensorGroup::getSensorAverageHumidity);
    }

    float getAverageVPD()
    {
      return this->getSafeAverageValue(&TemperatureHumiditySensorGroup::getSensorAverageVPD);
    }

    float getSafeAverageValue(float (*callback)(SensorType))
    {
      float total = 0;
      int count = 0;

      for (int i = 0; i < this->sensorCount; i++)
      {
        SensorType sensor = this->sensors[i];
        float value = (*callback)(sensor);

        if (sensor.isValid)
        {
          total += value;
          count++;
        }
      }

      this->hasErrors = count != this->sensorCount;

      return total / (float)count;
    }

    bool hasErrors;

    static float updateSensor(SensorType sensor)
    {
      return sensor.update();
    }

    static float getSensorHumidity(SensorType sensor)
    {
      return sensor.getHumidity();
    }

    static float getSensorTemp(SensorType sensor)
    {
      return sensor.getTemp();
    }

    static float getSensorVPD(SensorType sensor)
    {
      return sensor.getVPD();
    }

    static float getSensorAverageHumidity(SensorType sensor)
    {
      return sensor.getAverageHumidity();
    }

    static float getSensorAverageTemp(SensorType sensor)
    {
      return sensor.getAverageTemp();
    }

    static float getSensorAverageVPD(SensorType sensor)
    {
      return sensor.getVPD();
    }

  private:
    SensorType sensors[MAX_SENSORS];
    int sensorCount;
    movingAvg movingAverage;
  };

}

#endif
