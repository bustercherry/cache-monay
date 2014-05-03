clc
close all

%%
avgs = zeros(1, 10);
for i = 1:10
    temp = 100*(L2_hits(i:10:50)./(L2_hits(i:10:50) + L2_miss(i:10:50)));
    avg = sum(temp)/5;
    avgs(i) = avg;
end

x = 1:10;
hfig = figure;
bar(avgs, 0.5);

for i=1:numel(avgs)
  th(i) = text(x(i), avgs(i), sprintf('%.2f', avgs(i))); 
end

% center all annotations at once using the handles
set(th,'horizontalalignment', 'center', 'verticalalignment', 'bottom'); 
set(gca, 'xticklabel', config(1:10));
set(gca, 'XLim', [0, 11]);
set(hfig, 'Position', [500, 500, 850, 450]);
title('Average L2 hit percentage per config');
xlabel('Config');
ylabel('Percentage of L2 hits')

%%
hfig = figure;
y = cell(10, 1);
for i = 1:10
    y{i} = (write_cycles(i:10:50)./num_writes(i:10:50))';
end

y = cell2mat(y);
bar(1:length(y), y, 0.6)

ylabel('Cycles per write');
title('Write CPI by Config');
%set(th,'horizontalalignment', 'center', 'verticalalignment', 'bottom'); 
set(gca,'xticklabel', config(1:10))
set(gca, 'XLim', [0, 11]);
set(gca, 'YLim', [0, 50]);
set(hfig, 'Position', [500, 500, 850, 450]);
legend(trace1(1:10:50));

%%
hfig = figure;
y = cell(10, 1);
for i = 1:10
    y{i} = (read_cycles(i:10:50)./num_reads(i:10:50))';
end

y = cell2mat(y);
bar(1:length(y), y, 0.6)

ylabel('Cycles per read');
title('Read CPI by Config');
%set(th,'horizontalalignment', 'center', 'verticalalignment', 'bottom'); 
set(gca,'xticklabel', config(1:10))
set(gca, 'XLim', [0, 11]);
set(gca, 'YLim', [0, 30]);
set(hfig, 'Position', [500, 500, 850, 450]);
legend(trace1(1:10:50));

%%
hfig = figure;
y = cell(10, 1);
for i = 1:10
    y{i} = (tot_time(i:10:50)./num_inst(i:10:50))';
end

y = cell2mat(y);
bar(1:length(y), y, 0.6)

ylabel('Cycles per instruction');
title('Instruction CPI by Config');
%set(th,'horizontalalignment', 'center', 'verticalalignment', 'bottom'); 
set(gca,'xticklabel', config(1:10))
set(gca, 'XLim', [0, 11]);
set(gca, 'YLim', [0, 20]);
set(hfig, 'Position', [500, 500, 850, 450]);
legend(trace1(1:10:50));

%%
avgs = zeros(1, 10);
for i = 1:10
    avg = sum(tot_time(i:10:length(tot_time))/1E9)/5;
    avgs(i) = avg;
end

hfig = figure;
x = tot_cost(1:10);
scatter(avgs, x, 50, 1:10, 'fill')

for i=1:numel(avgs)
  th(i) = text(avgs(i), x(i), config(i)) ; 
end

% center all annotations at once using the handles
set(th,'horizontalalignment', 'center', 'verticalalignment', 'bottom') ; 

xlabel('Average Execution Time (billions of cycles)');
ylabel('Cost (dollars)');
title('Average Execution Time vs Cost');
set(hfig, 'Position', [500, 500, 850, 450]);
set(gca, 'XLim', [45 80])
%set(gca, 'xticklabel', config(1:10));

%%
avgs = zeros(1, 10);
for i = 1:10
    temp = tot_time(i:10:length(tot_time));
    avg = sum(temp)/5;
    avgs(i) = avg;
end

avgs = avgs/1E9;
x = 1:10;
hfig = figure;
bar(avgs, 0.5);

for i=1:numel(avgs)
  th(i) = text(x(i), avgs(i), sprintf('%.2f', avgs(i))) ; 
end

% center all annotations at once using the handles
set(th,'horizontalalignment', 'center', 'verticalalignment', 'bottom') ; 
set(gca, 'xticklabel', config(1:10));
set(gca, 'XLim', [0, 11]);
set(hfig, 'Position', [500, 500, 850, 450]);
title('Average execution time per config');
xlabel('Config');
ylabel('Time (billions of cycles)')
