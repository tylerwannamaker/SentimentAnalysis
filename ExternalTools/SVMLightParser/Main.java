import java.io.*;
import java.util.*;

public class Main {

    public static void main(String[] args) throws IOException {
      File filein = new File("C:\\Users\\T\\IdeaProjects\\expert to SVM parser\\src\\review_output_svm.txt");
      File fileout = new File("C:\\Users\\T\\IdeaProjects\\expert to SVM parser\\src\\new_review_output_svm.txt");
        if(!fileout.exists()){
            fileout.createNewFile();
        }


      BufferedReader br = new BufferedReader(new FileReader(filein));
      BufferedWriter bw = new BufferedWriter(new FileWriter(fileout));

      String st;
      int wordscount[] = new int[99999];
      int reviewnum=0;
      List<String> words= new ArrayList<String>();
        String[] arr;
      int step=0;
      final int minPosStars = 3;
      final int maxNegStars = 2;

      while((st = br.readLine())!=null){
          reviewnum++;
          String reviewOutput="";
          Arrays.fill(wordscount, 0);
          System.out.println();
                    if(Integer.parseInt(st)>=minPosStars){
                        bw.write(1+" ");
                    }else if(Integer.parseInt(st)<=maxNegStars){
                        bw.write(-1+" ");
                    }
                    st=br.readLine();
                    if(st.matches("end")){
                        break;
                    }
                    arr=st.split(" ");
                    for(String word : arr){
                        if(!words.contains(word)){
                            words.add(word);
                            wordscount[words.indexOf(word)]++;
                        }else{
                            wordscount[words.indexOf(word)]++;
                        }
                    }

                    for(int index = 0; index<wordscount.length;index++){
                        if(wordscount[index]>0){
                            //System.out.println("Index: "+index+" wordscount[index]: "+wordscount[index]+" length: "+arr.length);
                            reviewOutput+=(index + ":"+(((float)wordscount[index])/(float)arr.length)+" ");
                        }

            }

          bw.write(reviewOutput);
          bw.newLine();
      }

          bw.close();
          br.close();

    }
}
