using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace SP_Lab8
{
    public class Item
    {
        public string Filename { get; set; }
        public string Size { get; set; }
        public StorageFolder Folder { get; set; }
        public StorageFile File { get; set; }
        public override string ToString()
        {
            return Filename;
        }
    }

    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        StorageFolder previousLocation = null;
        StorageFolder currentFolder = Windows.ApplicationModel.Package.Current.InstalledLocation;
        private ObservableCollection<Item> filelist = new ObservableCollection<Item>();

        public StorageFolder CurrentFolder
        {
            get
            {
                return currentFolder;
            }

            set
            {
                previousLocation = currentFolder;
                currentFolder = value;
            }
        }

        public MainPage()
        {
            this.InitializeComponent();
            listView.ItemsSource = filelist;
            listView.ItemClick += ListView_ItemClick;
            PrintCwd();
        }

        private void ListView_ItemClick(object sender, ItemClickEventArgs e)
        {
            foreach (Item item in filelist)
            {
                if (e.ClickedItem.ToString() == item.Filename && item.Folder != null)
                {
                    CurrentFolder = item.Folder;
                    PrintCwd();
                    break;
                }
            }
            
        }

        async private void PrintCwd()
        {
            filelist.Clear();
            // add ..
            StorageFolder parent = await CurrentFolder.GetParentAsync();
            if (parent != null)
            {
                searchPattern.Text = "";
                filelist.Add(new Item { Filename = "..", Size = "FOLDER", Folder = parent });
            }
            else
            {
                searchPattern.Text = ("Lack of permissions :(");
                if (previousLocation != null)
                {
                    filelist.Add(new Item { Filename = "BACK", Size = "FOLDER", Folder = previousLocation });
                }
            }

            IReadOnlyList<StorageFolder> folderList = await CurrentFolder.GetFoldersAsync();

            foreach (StorageFolder folder in folderList)
            {
                filelist.Add(new Item { Filename = folder.DisplayName, Size = "FOLDER", Folder = folder});
            }

            IReadOnlyList<StorageFile> fileList = await CurrentFolder.GetFilesAsync();

            foreach (StorageFile file in fileList)
            {
                Windows.Storage.FileProperties.BasicProperties basicProperties = await file.GetBasicPropertiesAsync();
                string fileSize = ParseSize(basicProperties.Size);

                filelist.Add(new Item { Filename = file.DisplayName, Size = fileSize, File = file});
            }

            // move ..
            for (int i = 0; i < filelist.Count; ++i)
            {
                if (filelist[i].Filename == ".." || filelist[i].Filename == "BACK")
                {
                    Item item = filelist[i];
                    filelist.RemoveAt(i);
                    filelist.Insert(0, item);
                }
            }

            

            return;
        }

        private string ParseSize(ulong bytes)
        {
            double size = bytes;
            uint divided = 0;
            string result = "";

            while (size >= 1024)
            {
                size /= 1024;
                divided += 1;
            }

            switch (divided)
            {
                case 0:
                    result = string.Format("{0:n0} B", size);
                    break;
                case 1:
                    result = string.Format("{0:n0} KB", size);
                    break;
                case 2:
                    result = string.Format("{0:n0} MB", size);
                    break;
                case 3:
                    result = string.Format("{0:n0} GB", size);
                    break;
                case 4:
                    result = string.Format("{0:n0} TB", size);
                    break;
                case 5:
                    result = string.Format("{0:n0} PB", size);
                    break;
                default:
                    break;
            }

            return result;
        }

        async private void searchButton_Click(object sender, RoutedEventArgs e)
        {
            if (searchPattern.Text == "")
            {
                return;
            }

            filelist.Clear();
            // add ..
            filelist.Add(new Item { Filename = "<-- Close search result overview", Size = "FOLDER", Folder = CurrentFolder });

            Search(CurrentFolder);
        }

        async private void Search(StorageFolder folder)
        {
            // looking for files in cwd
            SearchFiles(folder);

            IReadOnlyList<StorageFolder> folderList = await folder.GetFoldersAsync();

            foreach (StorageFolder innerFolder in folderList)
            {
                Search(innerFolder);
            }
        }

        async private void SearchFiles(StorageFolder folder)
        {
            IReadOnlyList<StorageFile> fileList = await folder.GetFilesAsync();

            foreach (StorageFile file in fileList)
            {
                if (file.DisplayName.IndexOf(searchPattern.Text) >= 0)
                {
                    Windows.Storage.FileProperties.BasicProperties basicProperties = await file.GetBasicPropertiesAsync();
                    string fileSize = ParseSize(basicProperties.Size);

                    filelist.Add(new Item { Filename = file.DisplayName, Size = fileSize, File = file });
                }
            }
        }

        async private void cdButton_Click(object sender, RoutedEventArgs e)
        {
            var folderPicker = new Windows.Storage.Pickers.FolderPicker();
            folderPicker.SuggestedStartLocation = Windows.Storage.Pickers.PickerLocationId.Desktop;
            folderPicker.FileTypeFilter.Add("*");

            Windows.Storage.StorageFolder folder = await folderPicker.PickSingleFolderAsync();
            if (folder != null)
            {
                CurrentFolder = folder;
                PrintCwd();
            }
        }
    }
}
