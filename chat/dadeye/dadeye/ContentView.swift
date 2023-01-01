//
//  ContentView.swift
//  dadeye
//
//  Created by Elias Sun on 12/24/22.
//

import SwiftUI

struct ContentView: View {
    @StateObject
    private var deVM: DadEyeViewModel = .shared
    @State var options = ["dad", "sweet"]
    @State var selectedItem = "dad"
    @State private var sweetidfordad: String = ""
    @State private var showingAlert = false
    var body: some View {
        VStack{
            HStack{
                Text("My ID:").frame(alignment: .leading)
                Text(deVM.myID.description).frame(maxWidth: .infinity, alignment: .leading)
            }
            HStack{
                Picker("Select", selection: $selectedItem) {
                            ForEach(options, id:\.self) { name in
                                Text(name)
                            }
                        }
                .onReceive([self.selectedItem].publisher.first()) { value in
                    print("Selected ", selectedItem)
                 }

            }
            HStack{
                VStack {
                    HStack{
                        Text("Mode:").frame(alignment: .leading)
                        Text("Dad").frame(maxWidth: .infinity, alignment: .leading)
                    }
                    HStack{
                        Text("Dad ID:").frame(alignment: .leading)
                        Text(deVM.dadID.description).frame(maxWidth: .infinity, alignment: .leading)
                    }
                    HStack{
                        Text("Sweet ID:").frame(alignment: .leading)
                        TextEditor(text: $sweetidfordad).frame(height: 25)
                    }
                    HStack{
                        Text(deVM.dadStatus.description).frame(alignment: .leading)
                        Button {
                            DadEye().getscreen()
                        } label: {
                            Text("Sweet Screen")
                        }.controlSize(.regular)
                        Button {
                            DadEye().display()
                        } label: {
                            Text("Display")
                        }.controlSize(.regular).frame(maxWidth: .infinity)
                    }
                }
                VStack {
                    Text("   ")
                }
                VStack {
                    HStack{
                        Text("Mode:").frame(alignment: .leading)
                        Text("Sweet").frame(maxWidth: .infinity, alignment: .leading)
                    }
                    HStack{
                        Text("Sweet ID:").frame(alignment: .leading)
                        Text(deVM.sweetID.description).frame(maxWidth: .infinity, alignment: .leading)
                    }
                    HStack {
                        Text("Dad ID(Optional)").frame(maxWidth: .infinity, alignment: .leading)
                        TextEditor(text: .constant("Input Dad ID here")).frame(height: 25)
                    }
                    Text(deVM.sweetStatus.description).frame(maxWidth: .infinity, alignment: .leading)
                }
            }
            VStack{
                HStack {
                    Button {
                        //deVM.updateInfo(for: "Starting...")
                        if selectedItem == options[1] {
                            DadEye().sweetmode(deVM.dadID)
                        } else {
                            if sweetidfordad.lengthOfBytes(using: String.Encoding.ascii) != 8 {
                                showingAlert = true
                                return
                            }
                            DadEye().dadmode(sweetidfordad)
                        }
                    } label: {
                        Text("Start")
                    }.controlSize(.regular)
                        .alert("Please copy the correct sweet id and fill it to the sweet id box.", isPresented: $showingAlert) {
                                Button("OK", role: .cancel) { }

                    }
                    
                    Button {
                        exit(EXIT_SUCCESS)
                    } label: {
                        Text("Exit")
                    }.controlSize(.regular)
                }
            }
            VStack{
                HStack{
                    Text("Info: ").frame(alignment: .leading)
                    Text(deVM.info.description).frame(maxWidth: .infinity, alignment: .leading)
                }
            }
        }.onAppear(){
            DadEye().generateid()
        }.padding()
    }
    
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}


//MARK: View Model
final class DadEyeViewModel: ObservableObject {
    static let shared: DadEyeViewModel = .init()
    @Published
    private (set) var info: String = "Select \"dad\" or \"sweet\" mode, then click \"Start\" button to start."
    
    private (set) var dadStatus: String = "Status: Off"
    private (set) var dadID: String = "xxxx"
    private (set) var myID: String = "xxxx"
    var dadIDSet = false
    
    private (set) var sweetStatus: String = "Status: Off"
    private (set) var sweetID: String = "xxxx"
    
    @Published
    var isPresented: Bool = false
    func updateInfo(for value: String) {
        Task { @MainActor in
            info = value
            isPresented.toggle()
            print("update UI info", info.description)
        }
    }
    func updateAll(forinfo ninfo: String, fordad ndadid: String, forsweet nsweetid: String, formyid nmyid: String, fordadready ndadready: String, forsweetready nsweetready: String) {
        Task { @MainActor in
            if ninfo != "" {
                info = ninfo
            }
            if ndadid != "" {
                dadID = ndadid
            }
            if nsweetid != "" {
                sweetID = nsweetid
            }
            if nmyid != "" {
                myID = nmyid
            }
            if ndadready != "" {
                dadStatus = ndadready
            }
            if nsweetready != "" {
                sweetStatus = nsweetready
            }
            isPresented.toggle()
            print("update UI info", info.description)
        }
    }
    func updateDadID(for value: String) {
        dadIDSet = true
        Task { @MainActor in
            dadID = value
            print("update UI dadID", dadID.description)
        }
    }
    func updateSweetID(for value: String) {
        Task { @MainActor in
            sweetID = value
            print("update UI sweetID", sweetID.description)
        }
    }
}

let semaphore = DispatchSemaphore(value: 1)
var msgQ  : [String] = [""]
var codeQ : [UInt64] = [0]
@_cdecl("updateMainThread")
public func updateMainThread(code: UInt64, info:  UnsafePointer<UInt8>, size: UInt64) {
    if size == 0 {
        print("Invalid info")
        return
    }
    semaphore.wait()
    let str = String(cString: info)
    msgQ.append(str)
    codeQ.append(code)
    semaphore.signal()
    DispatchQueue.main.async {
        let codeBaseID   : UInt64   =  1000;
        let codeDadID    : UInt64   =  codeBaseID + 1;
        let codeSweetID  : UInt64   =  codeBaseID + 2;
        let codeExit     : UInt64   =  codeBaseID + 3;
        let codeDebug    : UInt64   =  codeBaseID + 4;
        let codeInfo     : UInt64   =  codeBaseID + 5;
        let codeWarning  : UInt64   =  codeBaseID + 6;
        let codeError    : UInt64   =  codeBaseID + 7;
        let codeCritical : UInt64   =  codeBaseID + 8;
        let codeMyID     : UInt64   =  codeBaseID + 9;
        let codeDadReady : UInt64   =  codeBaseID + 10;
        let codeSweetReady : UInt64 =  codeBaseID + 11;

        semaphore.wait()
        if codeQ.isEmpty {
            semaphore.signal()
            return
        }
        if msgQ.isEmpty {
            semaphore.signal()
            return
        }
        var ninfo : String = ""
        var ndadid : String = ""
        var nsweetid : String = ""
        var nmyid : String = ""
        var ndadready : String = ""
        var nsweetready : String = ""
        while !codeQ.isEmpty {
            let tq = codeQ.popLast()
            let tm = msgQ.popLast()
            if tq! < codeBaseID {
                continue
            }
            if codeDadID == tq {
                ndadid = tm!
            } else if codeSweetID == tq {
                nsweetid = tm!
            } else if codeMyID == tq {
                nmyid = tm!
            } else if codeDadReady == tq {
                ndadready = "Status: On"
                ninfo = tm!
            } else if codeSweetReady == tq {
                nsweetready = "Status: On"
                ninfo = tm!
            }else {
                ninfo = tm!
            }
        }
        DadEyeViewModel.shared.updateAll(forinfo: ninfo, fordad: ndadid, forsweet: nsweetid, formyid: nmyid, fordadready: ndadready, forsweetready: nsweetready)
        semaphore.signal()

//
//        DadEyeViewModel.shared.updateInfo(for: str)
//        DadEyeViewModel.shared.updateDadID(for: str)
    }
}


