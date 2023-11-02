using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Xml;
using System.IO;

namespace City
{
    class Program
    {
        static void Main(string[] args)
        {
            Client client = new Client();
            client.SetPath(); 
        }
    }
}