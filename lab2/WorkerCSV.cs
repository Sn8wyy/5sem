using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

using System.IO;
namespace City
{
    class WorkerCSV : IGetData, IPrintData
    {
        private const int cnt_floor = 5;
        private long processingTime;
        private Dictionary<(int, int, string, string), int> duplicates;
        private Dictionary<string, int[]> statistics;
        private string path;
        public void SetPath(string path) {
            this.path = path;
        }
        public void PrintFileProcessingTime() {
            Console.WriteLine("Processing time is {0} ms", processingTime);
        }
        public void PrintStatistics() {
            statistics = statistics.OrderBy(x => x.Key).ToDictionary(x => x.Key, x => x.Value);
            foreach (var value in statistics) {
                for (int i = 1; i <= cnt_floor; ++i) {
                    Console.WriteLine("В городе " + value.Key + " " + value.Value[i] + " " + i + "-этажных домов");
                }
            }
        }
        public void PrintDuplicates() {
            foreach (var value in duplicates) {
                if (value.Value > 1) {
                    Console.WriteLine("Запись: город " + value.Key.Item3 + " " + value.Key.Item4 +
                        " дом " + value.Key.Item2 + " c количеством этажей " + value.Key.Item1 +
                        " встречается в " + value.Value + " количестве");
                }
            }
        }
        public void GetData() {
            using (StreamReader reader = new StreamReader(path))
            {
                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();

                string line;
                reader.ReadLine(); // header

                const int cnt_floor = 5;
                statistics = new Dictionary<string, int[]>();
                Dictionary<(int, int, string, string), int> data = new Dictionary<(int, int, string, string), int>();

                while ((line = reader.ReadLine()) != null)  {
                    string[] words = line.Split(';');

                    string city = words[0];//.Split('"')[1];
                    string street = words[1];//.Split('"')[1];
                    int house = int.Parse(words[2]);
                    int floor = int.Parse(words[3]);

                    if (!statistics.ContainsKey(city)) statistics[city] = new int[cnt_floor + 1];
                    if (floor >= 1 && floor <= 5) ++statistics[city][floor];

                    var el2 = (floor, house, city, street);
                    data[el2] = 1 + (data.ContainsKey(el2) ? data[el2] : 0);
                }

                duplicates = new Dictionary<(int, int, string, string), int>();
                foreach (var value in data) {
                    if (value.Value > 1) duplicates.Add(value.Key, value.Value);
                }

                stopwatch.Stop();
                processingTime = stopwatch.ElapsedMilliseconds;
            }
        }
    }
}
