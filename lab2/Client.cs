using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;
namespace City
{
    class Client
    {
        private string path;
        public void SetPath()
        {
            while (path != "0")
            {
                Console.WriteLine("To exit, enter 0");
                Console.WriteLine("To receive data, enter the file path");
                path = Console.ReadLine();
                if (File.Exists(path) && path.Contains(".xml"))
                {
                    WorkerXML worker = new WorkerXML();
                    worker.SetPath(path);
                    worker.GetData();

                    worker.PrintStatistics();
                    Console.WriteLine();
                    worker.PrintDuplicates();
                    Console.WriteLine();
                    worker.PrintFileProcessingTime();
                }
                else if (File.Exists(path) && path.Contains(".csv"))
                {
                    WorkerCSV worker = new WorkerCSV();
                    worker.SetPath(path);
                    worker.GetData();

                    worker.PrintStatistics();
                    Console.WriteLine();
                    worker.PrintDuplicates();
                    Console.WriteLine();
                    worker.PrintFileProcessingTime();
                }
                else Console.WriteLine("File path error");
                Console.WriteLine();
            }
        }
    }
}
