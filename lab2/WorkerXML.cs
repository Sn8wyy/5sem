using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;
using System.Diagnostics;

namespace City
{
    class WorkerXML : IGetData, IPrintData
    {
        private const int cnt_floor = 5;
        private long processingTime;
        private Dictionary<(int, int, string, string), int> duplicates;
        private Dictionary<string, int[]> statistics;
        private string path;
        public void SetPath(string path)
        {
            this.path = path;
        }
        public void PrintFileProcessingTime()
        {
            Console.WriteLine("Processing time is {0} ms", processingTime);
        }
        public void PrintStatistics()
        {
            statistics = statistics.OrderBy(x => x.Key).ToDictionary(x => x.Key, x => x.Value);
            foreach (var value in statistics)
            {
                for (int i = 1; i <= cnt_floor; ++i)
                {
                    Console.WriteLine("В городе " + value.Key + " " + value.Value[i] + " " + i + "-этажных домов");
                }
            }
        }
        public void PrintDuplicates()
        {
            foreach (var value in duplicates)
            {
                if (value.Value > 1)
                {
                    Console.WriteLine("Запись: город " + value.Key.Item3 + " " + value.Key.Item4 +
                        " дом " + value.Key.Item2 + " c количеством этажей " + value.Key.Item1 +
                        " встречается в " + value.Value + " количестве");
                }
            }
        }
        public void GetData()
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            
            var reader = new XmlTextReader(path);
          
            const int cnt_floor = 5;
            statistics = new Dictionary<string, int[]>();
            Dictionary<(int, int, string, string), int> data = new Dictionary<(int, int, string, string), int>();
            while (reader.Read()) {
                if (reader.NodeType == XmlNodeType.Element)  {
                    if (reader.HasAttributes) {
                        reader.MoveToNextAttribute();
                        string city = reader.Value;
                        reader.MoveToNextAttribute();
                        string street = reader.Value;
                        reader.MoveToNextAttribute();
                        int house = int.Parse(reader.Value);
                        reader.MoveToNextAttribute();
                        int floor = int.Parse(reader.Value);

                        if (!statistics.ContainsKey(city)) statistics[city] = new int[cnt_floor + 1];
                        if (floor >= 1 && floor <= 5) ++statistics[city][floor];

                        var el2 = (floor, house, city, street);
                        data[el2] = 1 + (data.ContainsKey(el2) ? data[el2] : 0);
                    }
                }
            }

            duplicates = new Dictionary<(int, int, string, string), int>();
            foreach (var value in data) if (value.Value > 1) duplicates.Add(value.Key, value.Value);

            stopwatch.Stop();
            processingTime = stopwatch.ElapsedMilliseconds;
        }
    }
}
